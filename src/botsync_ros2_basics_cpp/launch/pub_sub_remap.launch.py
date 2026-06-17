from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    """
    Botsync Robotics — Publish/Subscribe with Remappings Launch File

    This launch file runs the publisher and subscriber nodes simultaneously,
    but isolates their communication by remapping the topic "botsync/topic"
    to "robot1/botsync/topic". This pattern allows running multiple identical
    robots in the same domain without message interference.
    """

    use_remap = LaunchConfiguration('use_remap')

    return LaunchDescription([
        DeclareLaunchArgument(
            'use_remap',
            default_value='true',
            description='Whether to remap botsync/topic into robot1/botsync/topic'
        ),

        Node(
            package='botsync_ros2_basics_cpp',
            executable='botsync_publisher',
            name='robot1_publisher',
            output='screen',
            remappings=[
                ('botsync/topic', 'robot1/botsync/topic'),
            ],
            condition=IfCondition(use_remap),
        ),

        Node(
            package='botsync_ros2_basics_cpp',
            executable='botsync_subscriber',
            name='robot1_subscriber',
            output='screen',
            remappings=[
                ('botsync/topic', 'robot1/botsync/topic'),
            ],
            condition=IfCondition(use_remap),
        ),
    ])