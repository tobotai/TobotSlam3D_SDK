# Service 详解 · Services (Application -> Robot)

应用 → 底盘 的调用接口。完整示例见 `examples/`。

| # | 功能 | Service | 请求类型 |
|---|---|---|---|
| 3.1 | 开始建图 | `/start_mapping_kitty` | `std_srvs/Trigger` |
| 3.2 | 保存地图 | `/save_map_kitty` | `kitty_runtime_manager/SaveMap` |
| 3.3 | 选择地图并启动导航 | `/select_map_and_start_kitty` | `kitty_runtime_manager/SaveMap` |
| 3.4 | 删除地图 | `/delete_map_kitty` | `kitty_runtime_manager/DeleteMap` |
| 3.5 | 回充电桩 | `/charge` | `std_srvs/Trigger` |
| 3.6 | 导航到目标点 | `/navigate_to_pose` | `nav_msgs/GetPlan` |
| 3.7 | 取消导航 | `/cancel_navigation` | `nav_msgs/GetPlan` |
| 3.8 | 添加目标点 | `/add_goal_kitty` | `kitty_runtime_manager/AddGoal` |
| 3.9 | 删除目标点 | `/delete_goal_kitty` | `kitty_runtime_manager/DeleteGoal` |

---

## 3.1 开始建图  std_srvs/Trigger


Service：  请求：  （空请求） 响应 values：  字段 类型 方向 说明 success bool 响应 操作是否成功 message string 响应 结果信息（失败时给原因）

```
/start_mapping_kitty
```

```
{}
```

```
{ "success": true, "message": "Mapping started" }
```

## 3.2 保存地图  kitty_runtime_manage


Service：  请求：  响应 values：  字段 类型 方向 说明 filepath string 请求 map_name（不要带路径） success bool 响应 操作是否成功 message string 响应 结果信息

```
/save_map_kitty
```

```
{ "filepath": "map_name" }
```

```
{ "success": true, "message": "Map saved" }
```

## 3.3 选择地图并启动导航  kitty_runtime_m


Service：  请求：  响应 values：  字段 类型 方向 说明 filepath string 请求 map_name（不要带路径） success bool 响应 操作是否成功 message string 响应 结果信息

```
/select_map_and_start_kitty
```

```
{ "filepath": "map_name" }
```

```
{ "success": true, "message": "Map selected and navigation started" }
```

## 3.4 删除地图  kitty_runtime_manage


Service：  请求：  响应 values：  字段 类型 方向 说明 map_name string 请求 要删除的map_name success bool 响应 操作是否成功 message string 响应 结果信息

```
/delete_map_kitty
```

```
{ "map_name": "map_name" }
```

```
{ "success": true, "message": "Deleted successfully" }
```

## 3.5 回充电桩  std_srvs/Trigger


Service：  请求：  响应 values：  字段 类型 方向 说明 success bool 响应 操作是否成功（只表示Docking流程已启动，不代表已充上电） message string 响应 结果信息

```
/docking_kitty
```

```
{}
```

```
{ "success": true, "message": "Docking started" }
```

## 3.6 导航到目标点  nav_msgs/GetPlan


Service：  请求： { "goal": { "pose": { "position": { "x": 1.0, "y": 2.0, "z": 0.0 } } } } 响应 values： { "plan": { "poses": [ {"pose":{"position":{"x":0,"y":0,"z":0}}}, {"pose":{"position":{"x":1,"y":2,"z":0}}} ]}} 字段 类型 方向 说明 goal.pose.position.x float64 请求 目标点 x（米） goal.pose.position.y float64 请求 目标点 y（米） goal.pose.position.z float64 请求 目标点 z（底盘恒传 0） plan.poses array[PoseStamped] 响应 规划出的路径点数组（可前端画路径） 注意： 是否真的到达，不要看本响应，要订阅  收到  。

```
/navigate_to_pose
```

```
{
  "goal": { "pose": { "position": { "x": 1.0, "y": 2.0, "z": 0.0 } } }
}
```

```
{ "plan": { "poses": [
  {"pose":{"position":{"x":0,"y":0,"z":0}}},
  {"pose":{"position":{"x":1,"y":2,"z":0}}}
]}}
```

```
/ebot_goal_status_local_planner
```

```
data[0]==1
```

## 3.7 取消导航  nav_msgs/GetPlan


Service：  （ 与导航同服务，特殊坐标区分 ） 请求： { "goal": { "pose": { "position": { "x": , "y": , "z":  } } } } 响应 values：  字段 类型 方向 说明 goal.pose.position.x float64 请求 固定  （取消标志） goal.pose.position.y float64 请求 固定  （取消标志） goal.pose.position.z float64 请求 固定  plan.poses array 响应 空或截断路径

```
/navigate_to_pose
```

```
{
  "goal": { "pose": { "position": { "x": 999999, "y": 888888, "z": 0 } } }
}
```

```
{ "plan": { "poses": [] } }
```

```
999999
```

```
888888
```

```
0
```

## 3.8 添加目标点  kitty_runtime_manag


Service：  请求： { "map_name": "Lobby", "goal_name": "Reception", "x": 1.0, "y": 2.0, "theta": 0.5 } 响应 values：  字段 类型 方向 说明 map_name string 请求 map_name goal_name string 请求 目标点名称 x float64 请求 目标点 x（米） y float64 请求 目标点 y（米） theta float64 请求 朝向角（弧度） success bool 响应 操作是否成功 message string 响应 结果信息

```
/add_goal_kitty
```

```
{
  "map_name": "Lobby",
  "goal_name": "Reception",
  "x": 1.0, "y": 2.0, "theta": 0.5
}
```

```
{ "success": true, "message": "Added successfully" }
```

## 3.9 删除目标点  kitty_runtime_manag


Service：  请求：  响应 values：  字段 类型 方向 说明 map_name string 请求 map_name goal_name string 请求 目标点名称 success bool 响应 操作是否成功 message string 响应 结果信息

```
/delete_goal_kitty
```

```
{ "map_name": "Lobby", "goal_name": "Reception" }
```

```
{ "success": true, "message": "Deleted successfully" }
```


---

## 命令行验证 · CLI Verification

npm install -g wscat wscat -c ws://192.168.1.151:9090 # Subscribe to battery state {"op":"subscribe","topic":"/ebot_work_state","type":"std_msgs/Float64MultiArray"} # Dock to charger, watch service_response {"op":"call_service","service":"/docking_kitty","args":{}} # Response: # {"op":"service_response","service":"/docking_kitty", # "values":{"success":true,"message":"Docking started"},"result":true} 技术支持：  ， 可索取工程模板与示例代码包。 ← 返回 Tbot 平台 Copyright © 2018-2026 上海瞳步智能科技有限公司 沪ICP备18016524号 document.querySelectorAll('.tabs').forEach(g=>{ const b=g.querySelectorAll('button'),p=g.parentElement.querySelectorAll('.tab-pane'); b.forEach((x,i)=>x.onclick=()=>{b.forEach(z=>z.classList.remove('active'));p.forEach(z=>z.classList.remove('active'));x.classList.add('active');p[i].classList.add('active');}); }); function toggleCode(btn){ var h3 = btn.closest('h3'); var pre = h3.parentElement.querySelector(':scope > h3:nth-of-type(' + (Array.prototype.indexOf.call(h3.parentElement.children, h3)+1) + ') ~ pre.code-block'); if(!pre){ pre = h3.nextElementSibling; while(pre && !pre.classList.contains('code-block')) pre = pre.nextElementSibling; } if(!pre) return; pre.classList.toggle('show'); btn.textContent = pre.classList.contains('show') ? (window.I18N?t('sdk_btn_collapse'):'收起代码') : (window.I18N?t('sdk_btn_expand_short'):'查看完整代码'); } const oldDetails = document.querySelectorAll('details > summary').forEach(s=>{ s.addEventListener('click',e=>{ e.preventDefault(); s.parentElement.toggleAttribute('open'); }); }); const ls=document.querySelectorAll('.sidebar a'),ss=[]; ls.forEach(a=>{const e=document.getElementById(a.getAttribute('href').slice(1));if(e)ss.push({e,a});}); window.addEventListener('scroll',()=>{let c=ss[0];ss.forEach(s=>{if(s.e.getBoundingClientRect().top a.classList.remove('active'));if(c)c.a.classList.add('active');}); 联系我们

```
support@tobotai.com
```
