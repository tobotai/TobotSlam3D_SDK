# cpp_demo.cpp 完整示例 — TOBOT AI SDK
# 依赖：vcpkg install websocketpp nlohmann-json
# 更多说明见 README.md 与 docs/
# 官网完整版：https://www.tobotai.com/sdk/index.html

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
using json=nlohmann::json;
typedef websocketpp::client<websocketpp::config::asio_client> WSC;
int main(){
  WSC c; c.init_asio();
  // Unified message handler: parse all Topic pushes and Service responses
  c.set_message_handler([](auto, WSC::message_ptr m){
    auto j=json::parse(m->get_payload());
    if(j["op"]=="publish"){
      std::string t=j["topic"];
      if(t=="/fast_lio_localization"){          // Localization
        auto p=j["msg"]["pose"]["pose"]["position"];
        auto o=j["msg"]["pose"]["pose"]["orientation"];
        std::cout<<"[Pose] x="<<p["x"]<<" y="<<p["y"]
                 <<" qw="<<o["w"]<<"\n";
      } else if(t=="/aft_pgo_path"){             // Global path
        std::cout<<"[Path] "<<j["msg"]["poses"].size()<<"  points\n";
      } else if(t=="/map_list_kitty"){           // Map list
        for(auto& mp: j["msg"]["maps"])
          std::cout<<"[Map] "<<mp["map_name"]<<"  goals_count="<<mp["goals"].size()<<"\n";
      } else if(t=="/ebot_goal_status_local_planner"){  // Arrival status
        if(j["msg"]["data"][0]==1) std::cout<<"✅ Arrived at goal\n";
      } else if(t=="/ebot_work_state"){          // Battery
        std::cout<<"[Battery] "<<j["msg"]["data"][1]<<"%"
                 <<(j["msg"]["data"][0]==1?" Charging":" Not charging")<<"\n";
      }
    } else if(j["op"]=="service_response"){     // Service response
      auto& v=j["values"];
      std::cout<<"[SVC] "<<j["service"]
               <<" success="<<v.value("success",false)
               <<" msg="<<v.value("message","")<<"\n";
    }
  });
  c.set_open_handler([&c](auto h){
    auto send=[&](json j){c.send(h,j.dump(),websocketpp::frame::opcode::text);};
    // Subscribe to 5 Topics
    send({{"op","subscribe"},{"topic","/fast_lio_localization"},{"type","nav_msgs/Odometry"}});
    send({{"op","subscribe"},{"topic","/aft_pgo_path"},{"type","nav_msgs/Path"}});
    send({{"op","subscribe"},{"topic","/map_list_kitty"},{"type","kitty_runtime_manager/MapList"}});
    send({{"op","subscribe"},{"topic","/ebot_goal_status_local_planner"},{"type","std_msgs/Float64MultiArray"}});
    send({{"op","subscribe"},{"topic","/ebot_work_state"},{"type","std_msgs/Float64MultiArray"}});
    // Call 9 Services
    send({{"op","call_service"},{"service","/start_mapping_kitty"},{"args",json::object()}});
    send({{"op","call_service"},{"service","/save_map_kitty"},{"args",{{"filepath","test_map"}}}});
    send({{"op","call_service"},{"service","/select_map_and_start_kitty"},{"args",{{"filepath","test_map"}}}});
    send({{"op","call_service"},{"service","/navigate_to_pose"},{"args",{{"goal",{{"pose",{{"position",{{"x",1.0},{"y",2.0},{"z",0.0}}}}}}}}}});
    send({{"op","call_service"},{"service","/navigate_to_pose"},{"args",{{"goal",{{"pose",{{"position",{{"x",999999.0},{"y",888888.0},{"z",0.0}}}}}}}}}});
    send({{"op","call_service"},{"service","/add_goal_kitty"},{"args",{{"map_name","test_map"},{"goal_name","front"},{"x",1.0},{"y",2.0},{"theta",0.5}}}});
    send({{"op","call_service"},{"service","/delete_goal_kitty"},{"args",{{"map_name","test_map"},{"goal_name","front"}}}});
    send({{"op","call_service"},{"service","/docking_kitty"},{"args",json::object()}});
    send({{"op","call_service"},{"service","/delete_map_kitty"},{"args",{{"map_name","test_map"}}}});
  });
  c.connect(c.get_connection("ws://192.168.1.151:9090").lock());
  c.run();
}
