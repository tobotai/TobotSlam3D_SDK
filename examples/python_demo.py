# python_demo.py 完整示例 — TOBOT AI SDK
# 依赖：pip install roslibpy
# 更多说明见 README.md 与 docs/
# 官网完整版：https://www.tobotai.com/sdk/index.html

import roslibpy, time, math
client = roslibpy.Ros(host='192.168.1.151', port=9090)
client.run()

# ===== Subscribe to all 5 Topics =====
# 1) Localization
def on_odom(m):
    p = m['pose']['pose']['position']; o = m['pose']['pose']['orientation']
    yaw = math.atan2(2*(o['w']*o['z']+o['x']*o['y']), 1-2*(o['z']**2+o['y']**2))
    print(f"[Pose] x={p['x']:.2f} y={p['y']:.2f} yaw={math.degrees(yaw):.1f}°")
roslibpy.Topic(client,'/fast_lio_localization','nav_msgs/Odometry').subscribe(on_odom)
# 2) Global path
roslibpy.Topic(client,'/aft_pgo_path','nav_msgs/Path').subscribe(
    lambda m: print(f"[Path] {len(m['poses'])}  points"))
# 3) Map list
roslibpy.Topic(client,'/map_list_kitty','kitty_runtime_manager/MapList').subscribe(
    lambda m: [print(f"[Map] {mp['map_name']}  goals:{[g['name'] for g in mp['goals']]}") for mp in m['maps']])
# 4) Navigation status
roslibpy.Topic(client,'/ebot_goal_status_local_planner','std_msgs/Float64MultiArray').subscribe(
    lambda m: m['data'][0]==1 and print("✅ Arrived"))
# 5) Battery
roslibpy.Topic(client,'/ebot_work_state','std_msgs/Float64MultiArray').subscribe(
    lambda m: print(f"[Battery] {m['data'][1]:.0f}% {'Charging' if m['data'][0]==1 else 'Not charging'}"))

# ===== Call all 9 Services =====
def call(svc, args, srv):
    r = roslibpy.Service(client, svc, srv).call(roslibpy.ServiceRequest(args))
    print(f"[SVC] {svc} → {dict(r)}")
time.sleep(1)
call('/start_mapping_kitty', {}, 'std_srvs/Trigger')
call('/save_map_kitty', {'filepath':'test_map'}, 'kitty_runtime_manager/SaveMap')
call('/select_map_and_start_kitty', {'filepath':'test_map'}, 'kitty_runtime_manager/SaveMap')
call('/navigate_to_pose', {'goal':{'pose':{'position':{'x':1.0,'y':2.0,'z':0.0}}}}, 'nav_msgs/GetPlan')
call('/navigate_to_pose', {'goal':{'pose':{'position':{'x':999999,'y':888888,'z':0}}}}, 'nav_msgs/GetPlan')
call('/add_goal_kitty', {'map_name':'test_map','goal_name':'Reception','x':1.0,'y':2.0,'theta':0.5}, 'kitty_runtime_manager/AddGoal')
call('/delete_goal_kitty', {'map_name':'test_map','goal_name':'Reception'}, 'kitty_runtime_manager/DeleteGoal')
call('/docking_kitty', {}, 'std_srvs/Trigger')
call('/delete_map_kitty', {'map_name':'test_map'}, 'kitty_runtime_manager/DeleteMap')

try:
    while True: time.sleep(1)
except KeyboardInterrupt: client.terminate()
