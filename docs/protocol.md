# 协议概览 · Protocol Overview

Tbot 底盘通过 **ROS over WebSocket（rosbridge）** 对外通信。连接机器人 IP 的 9090 端口即可订阅 Topic 与调用 Service，消息格式为标准 rosbridge JSON。

```
ws://<robot-ip>:9090
```

## 消息格式

```json
{"op":"service_response","service":"/xxx","values":{...},"result":true}
{"op":"subscribe","topic":"/xxx","type":"消息类型"}
```

## 快速验证

```bash
pip install roslibpy
npm install -g wscat
wscat -c ws://<robot-ip>:9090
```

---
方向 数量 名称 订阅 Topic（底盘 → 应用） 5 /fast_lio_localization、/aft_pgo_path、/map_list_kitty、/ebot_goal_status_local_planner、/ebot_work_state 调用 Service（应用 → 底盘） 9 /start_mapping_kitty、/save_map_kitty、/select_map_and_start_kitty、/delete_map_kitty、/docking_kitty、/navigate_to_pose（导航+取消）、/add_goal_kitty、/delete_goal_kitty

