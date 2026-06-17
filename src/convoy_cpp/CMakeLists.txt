cmake_minimum_required(VERSION 3.8)
project(convoy_cpp)


# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(convoy_interfaces REQUIRED)

add_executable(leader_node src/leader_node.cpp)
ament_target_dependencies(leader_node rclcpp convoy_interfaces)
add_executable(follower_node src/follower_node.cpp)
ament_target_dependencies(follower_node rclcpp convoy_interfaces)
add_executable(convoy_service src/convoy_service.cpp)
ament_target_dependencies(convoy_service rclcpp convoy_interfaces)

INSTALL(DIRECTORY launch/
  
  DESTINATION share/${PROJECT_NAME}/launch)

INSTALL (TARGETS
  leader_node
  follower_node
  convoy_service
  DESTINATION lib/${PROJECT_NAME})

ament_package()
