# go_demo.go 完整示例 — TOBOT AI SDK
# 依赖：go get github.com/gorilla/websocket
# 更多说明见 README.md 与 docs/
# 官网完整版：https://www.tobotai.com/sdk/index.html

package main
import ("encoding/json";"fmt";"log";
  "github.com/gorilla/websocket")
func sub(c *websocket.Conn, t, typ string){
  c.WriteJSON(map[string]interface{}{"op":"subscribe","topic":t,"type":typ})
}
func svc(c *websocket.Conn, name string, args map[string]interface{}){
  c.WriteJSON(map[string]interface{}{"op":"call_service","service":name,"args":args})
}
func main(){
  c,_,err:=websocket.DefaultDialer.Dial("ws://192.168.1.151:9090",nil)
  if err!=nil{log.Fatal(err)}
  defer c.Close()
  // Subscribe to 5 Topics
  sub(c,"/fast_lio_localization","nav_msgs/Odometry")
  sub(c,"/aft_pgo_path","nav_msgs/Path")
  sub(c,"/map_list_kitty","kitty_runtime_manager/MapList")
  sub(c,"/ebot_goal_status_local_planner","std_msgs/Float64MultiArray")
  sub(c,"/ebot_work_state","std_msgs/Float64MultiArray")
  // Call 9 Services
  svc(c,"/start_mapping_kitty",map[string]interface{}{})
  svc(c,"/save_map_kitty",map[string]interface{}{"filepath":"test_map"})
  svc(c,"/select_map_and_start_kitty",map[string]interface{}{"filepath":"test_map"})
  svc(c,"/navigate_to_pose",map[string]interface{}{"goal":map[string]interface{}{
    "pose":map[string]interface{}{"position":map[string]float64{"x":1,"y":2,"z":0}}}})
  svc(c,"/add_goal_kitty",map[string]interface{}{
    "map_name":"test_map","goal_name":"front","x":1.0,"y":2.0,"theta":0.5})
  svc(c,"/delete_goal_kitty",map[string]interface{}{"map_name":"test_map","goal_name":"front"})
  svc(c,"/docking_kitty",map[string]interface{}{})
  svc(c,"/delete_map_kitty",map[string]interface{}{"map_name":"test_map"})
  for{
    _,msg,_:=c.ReadMessage()
    var m map[string]interface{}
    json.Unmarshal(msg,&m)
    op,_:=m["op"].(string)
    if op=="publish"{
      t,_:=m["topic"].(string)
      md,_:=m["msg"].(map[string]interface{})
      switch t{
      case "/fast_lio_localization":
        p:=md["pose"].(map[string]interface{})["pose"].(map[string]interface{})["position"].(map[string]interface{})
        fmt.Printf("[Pose] x=%v y=%v\n",p["x"],p["y"])
      case "/aft_pgo_path":
        poses:=md["poses"].([]interface{})
        fmt.Printf("[Path] %d  points\n",len(poses))
      case "/map_list_kitty":
        for _,mp:=range md["maps"].([]interface{}){
          mpi:=mp.(map[string]interface{})
          fmt.Printf("[Map] %v  goals_count=%d\n",mpi["map_name"],len(mpi["goals"].([]interface{})))
        }
      case "/ebot_goal_status_local_planner":
        d:=md["data"].([]interface{})
        if d[0].(float64)==1 { fmt.Println("✅ Arrived at goal") }
      case "/ebot_work_state":
        d:=md["data"].([]interface{})
        fmt.Printf("[Battery] %.0f%% charging=%v\n",d[1],d[0])
      }
    } else if op=="service_response"{
      v:=m["values"].(map[string]interface{})
      fmt.Printf("[SVC] %v success=%v msg=%v\n",m["service"],v["success"],v["message"])
    }
  }
}
