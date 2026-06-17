from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    robot_name_arg = DeclareLaunchArgument(
        'robot_name',
        default_value='DefaultBot',
        description='Name of the robot reported in health messages'
    )

    health_publisher_node = Node(
        package='health_monitor_cpp',
        executable='health_publisher',
        name='health_publisher',
        parameters=[{'robot_name': LaunchConfiguration('robot_name')}],
        output='screen'
    )

    health_monitor_node = Node(
        package='health_monitor_cpp',
        executable='health_monitor',
        name='health_monitor',
        output='screen'
    )

    health_service_node = Node(
        package='health_monitor_cpp',
        executable='health_service',
        name='health_service',
        output='screen'
    )

    return LaunchDescription([
        robot_name_arg,
        health_publisher_node,
        health_monitor_node,
        health_service_node
    ])