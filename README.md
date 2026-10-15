# TOBOT AI SDK · Tbot 3D Navigation Chassis SDK

面向 **Tbot 全系移动底盘** 的开发者 SDK（ROS over WebSocket）。

The official developer SDK for **TOBOT (Tbot) mobile chassis** — ROS over WebSocket (rosbridge).

> 官网 SDK 页：<https://www.tobotai.com/sdk/index.html> · 技术支持：support@tobotai.com

## 协议 Protocol

所有底盘通过 **ROS over WebSocket** 通信，默认端口 **9090**：

```
ws://<robot-ip>:9090
```

消息为标准 rosbridge JSON（roslibpy / roslibjs 兼容）。

## 快速开始 Quick Start

```bash
# Python
pip install roslibpy

# 连接并订阅电量
import roslibpy
client = roslibpy.Ros(host='<robot-ip>', port=9090)
client.run()
roslibpy.Topic(client, '/ebot_work_state', 'std_msgs/Float64MultiArray').subscribe(
    lambda m: print("Battery", m['data'][1], "%"))
```

**7 种语言完整示例**（全部接口，开箱即用）：

| 语言 | 文件 | 依赖 |
|---|---|---|
| Python | [examples/python_demo.py](examples/python_demo.py) | roslibpy |
| JavaScript / HTML | [examples/javascript_demo.html](examples/javascript_demo.html) | 无（浏览器原生 WebSocket） |
| C++ | [examples/cpp_demo.cpp](examples/cpp_demo.cpp) | websocketpp, nlohmann-json |
| C | [examples/c_demo.c](examples/c_demo.c) | libwebsockets, cJSON |
| Java | [examples/java_demo.java](examples/java_demo.java) | Java-WebSocket, org.json |
| C# | [examples/csharp_demo.cs](examples/csharp_demo.cs) | Newtonsoft.Json |
| Go | [examples/go_demo.go](examples/go_demo.go) | gorilla/websocket |

## 接口一览 Interfaces

### Topic 订阅（底盘 → 应用）

| 功能 | Topic | 类型 |
|---|---|---|
| 机器人定位 | /fast_lio_localization | nav_msgs/Odometry |
| 全局路径 | /aft_pgo_path | nav_msgs/Path |
| 地图列表 | /map_list_kitty | kitty_runtime_manager/MapList |
| 导航状态 | /ebot_goal_status_local_planner | std_msgs/Float64MultiArray |
| 电量与充电状态 | /ebot_work_state | std_msgs/Float64MultiArray |

### Service 调用（应用 → 底盘）

| 功能 | Service | 类型 |
|---|---|---|
| 开始建图 | /start_mapping_kitty | std_srvs/Trigger |
| 保存地图 | /save_map_kitty | kitty_runtime_manager/SaveMap |
| 选择地图并启动导航 | /select_map_and_start_kitty | kitty_runtime_manager/SaveMap |
| 删除地图 | /delete_map_kitty | kitty_runtime_manager/DeleteMap |
| 回充电桩 | /charge | std_srvs/Trigger |
| 导航到目标点 | /navigate_to_pose | nav_msgs/GetPlan |
| 取消导航 | /cancel_navigation | nav_msgs/GetPlan |
| 添加目标点 | /add_goal_kitty | kitty_runtime_manager/AddGoal |
| 删除目标点 | /delete_goal_kitty | kitty_runtime_manager/DeleteGoal |

## 文档 Documentation

- [协议概览 · Protocol](docs/protocol.md)
- [Topic 详解 · Topics](docs/topics.md)
- [Service 详解 · Services](docs/services.md)

## 命令行验证 CLI

```bash
npm install -g wscat
wscat -c ws://<robot-ip>:9090
```

## 支持 Support

- 技术支持：support@tobotai.com（工作日 1 小时内响应）
- 官网：<https://www.tobotai.com>
- 选型与规格：[PRODUCT-SPECIFICATIONS](https://github.com/tobotai/tobotai)（主仓库）

## License

[Apache-2.0](LICENSE)

*（c）上海瞳步机器人科技有限公司 · TOBOT AI*
