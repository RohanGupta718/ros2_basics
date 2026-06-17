#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <string>

class BotsyncParamNode : public rclcpp::Node
{
public:
    BotsyncParamNode() : Node("botsync_param_node")
    {
        // Declare parameters with default values
        this->declare_parameter<std::string>("robot_name", "BotsyncBot");
        this->declare_parameter<double>("publish_rate_hz", 2.0);
        this->declare_parameter<int>("max_messages", 100);

        // Read the parameter values
        robot_name_ = this->get_parameter("robot_name").as_string();
        publish_rate_hz_ = this->get_parameter("publish_rate_hz").as_double();
        max_messages_ = this->get_parameter("max_messages").as_int();

        RCLCPP_INFO(this->get_logger(), "Robot Name: %s", robot_name_.c_str());
        RCLCPP_INFO(this->get_logger(), "Publish Rate: %.1f Hz", publish_rate_hz_);
        RCLCPP_INFO(this->get_logger(), "Max Messages: %d", max_messages_);

        auto period_ms = std::chrono::milliseconds(
            static_cast<int>(1000.0 / publish_rate_hz_)
        );

        timer_ = this->create_wall_timer(
            period_ms,
            std::bind(&BotsyncParamNode::timerCallback, this)
        );
    }

private:
    void timerCallback()
    {
        // Utilize the configurable parameter when reporting state
        RCLCPP_INFO(this->get_logger(), "[%s] Timer tick!", robot_name_.c_str());
    }
    std::string robot_name_;
    double publish_rate_hz_;
    int max_messages_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    // Begin ROS 2 execution
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BotsyncParamNode>());
    // Trigger controlled shutdown sequence to clean up resources
    rclcpp::shutdown();
    return 0;
}
