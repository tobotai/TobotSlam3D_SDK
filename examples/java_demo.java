# java_demo.java 完整示例 — TOBOT AI SDK
# 依赖：org.java-websocket:Java-WebSocket / org.json:json
# 更多说明见 README.md 与 docs/
# 官网完整版：https://www.tobotai.com/sdk/index.html

import org.java_websocket.client.*;
import org.java_websocket.handshake.ServerHandshake;
import org.json.JSONObject;
import java.net.URI;
public class Tbot {
  static WebSocketClient ws;
  static void sub(String t,String type){
    ws.send(new JSONObject().put("op","subscribe").put("topic",t).put("type",type).toString());
  }
  static void svc(String n,JSONObject args){
    ws.send(new JSONObject().put("op","call_service").put("service",n).put("args",args).toString());
  }
  public static void main(String[] a) throws Exception {
    ws = new WebSocketClient(new URI("ws://192.168.1.151:9090")) {
      public void onOpen(ServerHandshake h){
        sub("/fast_lio_localization","nav_msgs/Odometry");
        sub("/aft_pgo_path","nav_msgs/Path");
        sub("/map_list_kitty","kitty_runtime_manager/MapList");
        sub("/ebot_goal_status_local_planner","std_msgs/Float64MultiArray");
        sub("/ebot_work_state","std_msgs/Float64MultiArray");
        svc("/start_mapping_kitty", new JSONObject());
        svc("/save_map_kitty", new JSONObject().put("filepath","test_map"));
        svc("/select_map_and_start_kitty", new JSONObject().put("filepath","test_map"));
        JSONObject pos=new JSONObject().put("x",1.0).put("y",2.0).put("z",0.0);
        JSONObject goal=new JSONObject().put("pose",new JSONObject().put("position",pos));
        svc("/navigate_to_pose", new JSONObject().put("goal",goal));
        svc("/add_goal_kitty", new JSONObject().put("map_name","test_map").put("goal_name","front")
            .put("x",1.0).put("y",2.0).put("theta",0.5));
        svc("/delete_goal_kitty", new JSONObject().put("map_name","test_map").put("goal_name","front"));
        svc("/docking_kitty", new JSONObject());
        svc("/delete_map_kitty", new JSONObject().put("map_name","test_map"));
      }
      public void onMessage(String s){
        JSONObject m=new JSONObject(s);
        String op=m.optString("op");
        if("publish".equals(op)){
          String t=m.optString("topic");
          JSONObject msg=m.getJSONObject("msg");
          if("/fast_lio_localization".equals(t)){
            JSONObject p=msg.getJSONObject("pose").getJSONObject("pose").getJSONObject("position");
            System.out.println("[Pose] x="+p.getDouble("x")+" y="+p.getDouble("y"));
          } else if("/aft_pgo_path".equals(t)){
            System.out.println("[Path] "+msg.getJSONArray("poses").length()+"  points");
          } else if("/map_list_kitty".equals(t)){
            var maps=msg.getJSONArray("maps");
            for(int i=0;i<maps.length();i++){
              JSONObject mp=maps.getJSONObject(i);
              System.out.println("[Map] "+mp.getString("map_name")+"  goals_count="+mp.getJSONArray("goals").length());
            }
          } else if("/ebot_goal_status_local_planner".equals(t)){
            if(msg.getJSONArray("data").getDouble(0)==1) System.out.println("✅ Arrived at goal");
          } else if("/ebot_work_state".equals(t)){
            var d=msg.getJSONArray("data");
            System.out.println("[Battery] "+d.getDouble(1)+"% "+(d.getDouble(0)==1?"Charging":"Not charging"));
          }
        } else if("service_response".equals(op)){
          JSONObject v=m.getJSONObject("values");
          System.out.println("[SVC] "+m.getString("service")+" success="+v.optBoolean("success")+" msg="+v.optString("message"));
        }
      }
      public void onClose(int c,String r,boolean b){}
      public void onError(Exception e){ e.printStackTrace(); }
    };
    ws.connectBlocking();
  }
}
