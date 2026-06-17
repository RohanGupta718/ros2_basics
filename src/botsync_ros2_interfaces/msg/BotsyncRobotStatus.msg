# botsync_ros2_interfaces/msg/BotsyncRobotStatus.msg
std_msgs/Header header
string robot_id
float32 battery_percentage
float32 cpu_usage
bool is_operational
geometry_msgs/Pose current_pose