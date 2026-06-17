cmake_minimum_required(VERSION 3.8)
project(botsync_ros2_basics_cpp)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(botsync_ros2_interfaces REQUIRED)
find_package(rclcpp_action REQUIRED)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_calc_logic test/test_calc_logic.cpp)
  target_include_directories(test_calc_logic PUBLIC include)
endif()

add_executable(hello_world src/hello_world.cpp)
ament_target_dependencies(hello_world rclcpp)
add_executable(botsync_publisher src/publisher.cpp)
ament_target_dependencies(botsync_publisher rclcpp std_msgs)
add_executable(botsync_subscriber src/subscriber.cpp)
ament_target_dependencies(botsync_subscriber rclcpp std_msgs)
add_executable(botsync_param_node src/param_node.cpp)
ament_target_dependencies(botsync_param_node rclcpp)
add_executable(param_node_dynamic src/param_node_dynamic.cpp)
ament_target_dependencies(param_node_dynamic rclcpp rcl_interfaces)
add_executable(botsync_service_server src/service_server.cpp)
ament_target_dependencies(botsync_service_server rclcpp botsync_ros2_interfaces)
add_executable(botsync_service_client src/service_client.cpp)
ament_target_dependencies(botsync_service_client rclcpp botsync_ros2_interfaces)
add_executable(move_distance_server src/move_distance_server.cpp)
ament_target_dependencies(move_distance_server rclcpp rclcpp_action botsync_ros2_interfaces)
add_executable(move_distance_client src/move_distance_client.cpp)
ament_target_dependencies(move_distance_client rclcpp rclcpp_action botsync_ros2_interfaces)

install(TARGETS
  hello_world
  botsync_publisher
  botsync_subscriber
  botsync_param_node
  param_node_dynamic
  botsync_service_server
  botsync_service_client
  move_distance_server
  move_distance_client
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch)

ament_package()