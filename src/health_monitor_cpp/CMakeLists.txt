cmake_minimum_required(VERSION 3.8)
project(health_monitor_cpp)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(health_interfaces REQUIRED)

add_executable(health_publisher src/health_publisher.cpp)
ament_target_dependencies(health_publisher rclcpp std_msgs health_interfaces)
add_executable(health_monitor src/health_monitor.cpp)
ament_target_dependencies(health_monitor rclcpp std_msgs health_interfaces)
add_executable(health_service src/health_service.cpp)
ament_target_dependencies(health_service rclcpp std_msgs health_interfaces)

install(TARGETS 
  health_publisher
  health_monitor
  health_service
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch)

ament_package()