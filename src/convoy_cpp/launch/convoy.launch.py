from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    leader_name = LaunchConfiguration('leader_name')
    return LaunchDescription([
        DeclareLaunchArgument(
            'leader_name',
            default_value='Leader1',
            description='Name of the leader robot'
        ),

        Node(
            package='convoy_cpp',
            executable='leader_node',
            name='leader_node',
            output='screen',
            parameters=[{'leader_name': leader_name}]
        ),

        Node(
            package='convoy_cpp',
            executable='follower_node',
            name='follower_node',
            output='screen',
        ),

        Node(
            package='convoy_cpp',
            executable='convoy_service',
            name='convoy_service',
            output='screen',
        ),
    ])