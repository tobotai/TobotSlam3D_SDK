# c_demo.c 完整示例 — TOBOT AI SDK
# 依赖：libwebsockets / cJSON
# 更多说明见 README.md 与 docs/
# 官网完整版：https://www.tobotai.com/sdk/index.html

#include <libwebsockets.h>
#include <cjson/cJSON.h>
#include <string.h>
// Send JSON to rosbridge
static void send_json(struct lws*wsi,const char*s){
  unsigned char buf[LWS_PRE+1024];
  memcpy(&buf[LWS_PRE],s,strlen(s));
  lws_write(wsi,&buf[LWS_PRE],strlen(s),LWS_WRITE_TEXT);
}
static int cb(struct lws*wsi, enum lws_callback_reasons r, void*u, void*in, size_t n){
  if(r==LWS_CALLBACK_CLIENT_ESTABLISHED){
    // ---- Subscribe to 5 Topics ----
    send_json(wsi,"{\"op\":\"subscribe\",\"topic\":\"/fast_lio_localization\",\"type\":\"nav_msgs/Odometry\"}");
    send_json(wsi,"{\"op\":\"subscribe\",\"topic\":\"/aft_pgo_path\",\"type\":\"nav_msgs/Path\"}");
    send_json(wsi,"{\"op\":\"subscribe\",\"topic\":\"/map_list_kitty\",\"type\":\"kitty_runtime_manager/MapList\"}");
    send_json(wsi,"{\"op\":\"subscribe\",\"topic\":\"/ebot_goal_status_local_planner\",\"type\":\"std_msgs/Float64MultiArray\"}");
    send_json(wsi,"{\"op\":\"subscribe\",\"topic\":\"/ebot_work_state\",\"type\":\"std_msgs/Float64MultiArray\"}");
    // ---- Call 9 Services ----
    send_json(wsi,"{\"op\":\"call_service\",\"service\":\"/start_mapping_kitty\",\"args\":{}}");
    send_json(wsi,"{\"op\":\"call_service\",\"service\":\"/save_map_kitty\",\"args\":{\"filepath\":\"test_map\"}}");
    send_json(wsi,"{\"op\":\"call_service\",\"service\":\"/select_map_and_start_kitty\",\"args\":{\"filepath\":\"test_map\"}}");
    send_json(wsi,"{\"op\":\"call_service\",\"service\":\"/navigate_to_pose\",\"args\":{\"goal\":{\"pose\":{\"position\":{\"x\":1,\"y\":2,\"z\":0}}}}}");
    send_json(wsi,"{\"op\":\"call_service\",\"service\":\"/add_goal_kitty\",\"args\":{\"map_name\":\"test_map\",\"goal_name\":\"front\",\"x\":1,\"y\":2,\"theta\":0.5}}");
    send_json(wsi,"{\"op\":\"call_service\",\"service\":\"/delete_goal_kitty\",\"args\":{\"map_name\":\"test_map\",\"goal_name\":\"front\"}}");
    send_json(wsi,"{\"op\":\"call_service\",\"service\":\"/docking_kitty\",\"args\":{}}");
    send_json(wsi,"{\"op\":\"call_service\",\"service\":\"/delete_map_kitty\",\"args\":{\"map_name\":\"test_map\"}}");
  }
  if(r==LWS_CALLBACK_CLIENT_RECEIVE){
    cJSON*root=cJSON_ParseWithLength((char*)in,n);
    if(root){
      cJSON*op=cJSON_GetObjectItemCaseSensitive(root,"op");
      if(op && cJSON_IsString(op)){
        if(!strcmp(op->valuestring,"publish")){
          cJSON*topic=cJSON_GetObjectItemCaseSensitive(root,"topic");
          cJSON*msg=cJSON_GetObjectItemCaseSensitive(root,"msg");
          if(!strcmp(topic->valuestring,"/ebot_work_state")){
            cJSON*data=cJSON_GetObjectItemCaseSensitive(msg,"data");
            // data[0]: 1=charging 2=not charging  data[1]: battery %
            printf("[Battery] %.0f%% %s\n",
              cJSON_GetArrayItem(data,1)->valuedouble,
              cJSON_GetArrayItem(data,0)->valuedouble==1?"Charging":"Not charging");
          } else if(!strcmp(topic->valuestring,"/ebot_goal_status_local_planner")){
            if(cJSON_GetObjectItemCaseSensitive(msg,"data")->child->valuedouble==1)
              printf("Arrived at goal\n");
          } else if(!strcmp(topic->valuestring,"/fast_lio_localization")){
            cJSON*p=cJSON_GetObjectItemCaseSensitive(msg,"pose")->child->child;
            printf("[Pose] x=%.2f y=%.2f\n",
              cJSON_GetObjectItemCaseSensitive(p,"x")->valuedouble,
              cJSON_GetObjectItemCaseSensitive(p,"y")->valuedouble);
          }
        } else if(!strcmp(op->valuestring,"service_response")){
          cJSON*svc=cJSON_GetObjectItemCaseSensitive(root,"service");
          cJSON*val=cJSON_GetObjectItemCaseSensitive(root,"values");
          cJSON*succ=cJSON_GetObjectItemCaseSensitive(val,"success");
          cJSON*msg=cJSON_GetObjectItemCaseSensitive(val,"message");
          printf("[SVC] %s success=%d msg=%s\n",
            svc->valuestring, succ?succ->valueint:0, msg?msg->valuestring:"");
        }
      }
      cJSON_Delete(root);
    }
  }
  return 0;
}
