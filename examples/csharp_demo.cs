# csharp_demo.cs 完整示例 — TOBOT AI SDK
# 依赖：dotnet add package Newtonsoft.Json
# 更多说明见 README.md 与 docs/
# 官网完整版：https://www.tobotai.com/sdk/index.html

using System; using System.Net.WebSockets; using System.Text;
using System.Threading; using System.Threading.Tasks;
using Newtonsoft.Json.Linq;
class Tbot {
  static ClientWebSocket ws;
  static async Task Send(string msg){
    var b=Encoding.UTF8.GetBytes(msg);
    await ws.SendAsync(new ArraySegment<byte>(b),WebSocketMessageType.Text,true,CancellationToken.None);
  }
  static async Task Main() {
    ws=new ClientWebSocket();
    await ws.ConnectAsync(new Uri("ws://192.168.1.151:9090"),CancellationToken.None);
    // Subscribe to 5 Topics
    await Send("{\"op\":\"subscribe\",\"topic\":\"/fast_lio_localization\",\"type\":\"nav_msgs/Odometry\"}");
    await Send("{\"op\":\"subscribe\",\"topic\":\"/aft_pgo_path\",\"type\":\"nav_msgs/Path\"}");
    await Send("{\"op\":\"subscribe\",\"topic\":\"/map_list_kitty\",\"type\":\"kitty_runtime_manager/MapList\"}");
    await Send("{\"op\":\"subscribe\",\"topic\":\"/ebot_goal_status_local_planner\",\"type\":\"std_msgs/Float64MultiArray\"}");
    await Send("{\"op\":\"subscribe\",\"topic\":\"/ebot_work_state\",\"type\":\"std_msgs/Float64MultiArray\"}");
    // Call 9 Services
    await Send("{\"op\":\"call_service\",\"service\":\"/start_mapping_kitty\",\"args\":{}}");
    await Send("{\"op\":\"call_service\",\"service\":\"/save_map_kitty\",\"args\":{\"filepath\":\"test_map\"}}");
    await Send("{\"op\":\"call_service\",\"service\":\"/select_map_and_start_kitty\",\"args\":{\"filepath\":\"test_map\"}}");
    await Send("{\"op\":\"call_service\",\"service\":\"/navigate_to_pose\",\"args\":{\"goal\":{\"pose\":{\"position\":{\"x\":1,\"y\":2,\"z\":0}}}}}");
    await Send("{\"op\":\"call_service\",\"service\":\"/add_goal_kitty\",\"args\":{\"map_name\":\"test_map\",\"goal_name\":\"front\",\"x\":1,\"y\":2,\"theta\":0.5}}");
    await Send("{\"op\":\"call_service\",\"service\":\"/delete_goal_kitty\",\"args\":{\"map_name\":\"test_map\",\"goal_name\":\"front\"}}");
    await Send("{\"op\":\"call_service\",\"service\":\"/docking_kitty\",\"args\":{}}");
    await Send("{\"op\":\"call_service\",\"service\":\"/delete_map_kitty\",\"args\":{\"map_name\":\"test_map\"}}");
    var buf=new byte[8192];
    while(true){
      var r=await ws.ReceiveAsync(new ArraySegment<byte>(buf),CancellationToken.None);
      var m=JObject.Parse(Encoding.UTF8.GetString(buf,0,r.Count));
      if(m["op"]?.ToString()=="publish" && m["topic"]?.ToString()=="/ebot_work_state"){
        var d=(JArray)m["msg"]["data"];
        Console.WriteLine($"[Battery] {d[1]}% charging={d[0]}");
      }
      if(m["op"]?.ToString()=="service_response")
        Console.WriteLine($"[SVC] {m["service"]} → {m["values"]}");
    }
  }
}
