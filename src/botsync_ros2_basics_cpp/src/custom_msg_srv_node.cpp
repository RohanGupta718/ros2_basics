#include "rclcpp/rclcpp.hpp"
#include "botsync_ros2_interfaces/msg/botsync_robot_help.hpp"
#include <chrono>
#include <functional>
#include <memory>
class CustomMessagingNode : public rclcpp::Node {
    public:
        CustomMessagingNode() : Node(”custom_message_demonstration”)
        {
    // 1. Standup publisher connected to newly forged interface schemas
            publisher_ = this->create_publisher<botsync_ros2_interfaces::msg::BotsyncRobotStatus>("botsync_robot_status", 10);
    // 2. Drive repeated timer executions outputting structures
            timer_ = this->create_wall_timer(
                std::chrono::milliseconds(1000), // 1 Hz fixed rate
                std::bind(&CustomMessagingNode::publishCustomData, this)
            );
            RCLCPP_INFO(this->get_logger(), ”Custom messaging publisher spinning!”);
        }
private:
    void publishCustomData()
    {
        auto status_msg = botsync_ros2_interfaces::msg::BotsyncRobotStatus();
        // Populate standard primitive mappings directly translating .msg syntax
        status_msg.header.stamp = this->get_clock()->now();
        status_msg.header.frame_id = ”base_link”;
        status_msg.robot_id = ”botsync_alpha_bot”;
        status_msg.battery_percentage = 85.3f;
        status_msg.cpu_usage = 23.4f;
        status_msg.is_operational = true;
        // Pushing geometry component mapping (x,y,z spatial awareness geometry_msgs)
        status_msg.current_pose.position.x = 10.5;
        status_msg.current_pose.position.y = -5.0;
        status_msg.current_pose.position.z = 0.0;
        // Calculate quaternion (assuming fixed 0 rotation baseline)
        status_msg.current_pose.orientation.w = 1.0;
        // Announce locally on terminal visually inspecting transmission
        RCLCPP_INFO(this->get_logger(), ”Sending custom diagnostics for: %s | Batt: %.1f%%”,
                status_msg.robot_id.c_str(), status_msg.battery_percentage);
        // Submit the complex message across the standard communication framework safely
        publisher_->publish(status_msg);
    }
    rclcpp::Publisher<botsync_ros2_interfaces::msg::BotsyncRobotStatus>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CustomMessagingNode>());
    rclcpp::shutdown();
    return 0;
}
