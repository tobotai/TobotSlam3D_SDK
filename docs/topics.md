# Topic 详解 · Topics (Robot -> Application)

底盘 → 应用 的订阅接口。完整示例见 `examples/`。

| # | 功能 | Topic | 消息类型 |
|---|---|---|---|
| 2.1 | 机器人定位 | `/fast_lio_localization` | `nav_msgs/Odometry` |
| 2.2 | 全局路径 | `/aft_pgo_path` | `nav_msgs/Path` |
| 2.3 | 地图列表 | `/map_list_kitty` | `kitty_runtime_manager/MapList` |
| 2.4 | 导航状态 | `/ebot_goal_status_local_planner` | `std_msgs/Float64MultiArray` |
| 2.5 | 电量与充电状态 | `/ebot_work_state` | `std_msgs/Float64MultiArray` |

---

## 2.1 机器人定位  nav_msgs/Odometry


Topic：  ，底盘定时推送当前位姿。 收到的 msg： { "pose": { "pose": { "position": {"x": 1.23, "y": 4.56, "z": 0.0}, "orientation": {"x": 0, "y": 0, "z": 0.5, "w": 0.866} }} } 字段 类型 说明 pose.pose.position.x float64 机器人 x 坐标（米） pose.pose.position.y float64 机器人 y 坐标（米） pose.pose.position.z float64 机器人 z 坐标（米，底盘固定 0） pose.pose.orientation.x/y/z/w float64 四元数朝向；前端用  转弧度角

```
/fast_lio_localization
```

```
{
  "pose": { "pose": {
    "position": {"x": 1.23, "y": 4.56, "z": 0.0},
    "orientation": {"x": 0, "y": 0, "z": 0.5, "w": 0.866}
  }}
}
```

```
yaw = atan2(2*(w*z+x*y), 1-2*(z*z+y*y))
```

## 2.2 全局路径  nav_msgs/Path


Topic：  。启动导航后约 5 秒发一次，共发 5 次（约 30 秒）。 收到的 msg： { "poses": [ {"pose":{"position":{"x":0,"y":0,"z":0}}}, {"pose":{"position":{"x":1,"y":2,"z":0}}} ]} 字段 类型 说明 poses array[PoseStamped] 规划路径点数组 poses[].pose.position.x float64 路径点 x 坐标（米） poses[].pose.position.y float64 路径点 y 坐标（米） poses[].pose.position.z float64 路径点 z 坐标（底盘恒为 0）

```
/aft_pgo_path
```

```
{ "poses": [
  {"pose":{"position":{"x":0,"y":0,"z":0}}},
  {"pose":{"position":{"x":1,"y":2,"z":0}}}
]}
```

## 2.3 地图列表  kitty_runtime_manage


Topic：  。订阅后返回所有地图及其已保存目标点。 收到的 msg： { "maps": [ {"map_name":"Lobby","map_path":"/maps/floor1", "goals":[ {"name":"Reception","x":1.0,"y":2.0,"theta":0.5}, {"name":"电梯口","x":3.0,"y":4.0,"theta":1.2} ]} ]} 字段 类型 说明 maps array[Map] 地图数组 maps[].map_name string 地图显示名称 maps[].map_path string 地图文件路径（内部用） maps[].goals array[Goal] 该地图下已保存目标点 maps[].goals[].name string 目标点名称 maps[].goals[].x float64 目标点 x（米） maps[].goals[].y float64 目标点 y（米） maps[].goals[].theta float64 目标点朝向（弧度）

```
/map_list_kitty
```

```
{ "maps": [
  {"map_name":"Lobby","map_path":"/maps/floor1",
   "goals":[
     {"name":"Reception","x":1.0,"y":2.0,"theta":0.5},
     {"name":"电梯口","x":3.0,"y":4.0,"theta":1.2}
   ]}
]}
```

## 2.4 导航状态  std_msgs/Float64Mult


Topic：  。 收到的 msg：  字段 类型 说明 data[] float64  = Arrived at goal；  = 未到达。Docking场景下只有真正充上电才发

```
/ebot_goal_status_local_planner
```

```
{"data": [1]}
```

```
1
```

```
0
```

## 2.5 电量与充电状态  std_msgs/Float64M


Topic：  。 收到的 msg：  字段 类型 说明 data[0] float64 充电状态：  = Charging，  = Not charging data[] float64 电量百分比（0~00）

```
/ebot_work_state
```

```
{"data": [2, 75.5]}
```

```
1
```

```
2
```

