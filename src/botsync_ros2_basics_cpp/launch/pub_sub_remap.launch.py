from launch import LaunchDescription
from launch_ros.actions import Node
def generate_launch_description():
publisher_node = Node(
package='botsync_ros2_basics_cpp',
executable='botsync_publisher',
name='botsync_publisher',
output='screen',
)
subscriber_node = Node(
package='botsync_ros2_basics_cpp',
executable='botsync_subscriber',
name='botsync_subscriber',
output='screen',
)
return LaunchDescription([
publisher_node,
subscriber_node,
])