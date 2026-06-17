#include "rclcpp/rclcpp.hpp"
#include "health_interfaces/msg/robot_health.hpp"
#include <functional>
#include <memory>

class HealthMonitor : public rclcpp::Node {
public:
    HealthMonitor() : Node("health_monitor") {
        subscription_ = this->create_subscription<health_interfaces::msg::RobotHealth>(
            "robot_health",
            10,
            std::bind(&HealthMonitor::healthCallback, this, std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "Health Monitor has been started.");
    }

private:
    void healthCallback(const health_interfaces::msg::RobotHealth::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "[%s] battery=%.1f%% temp=%.1fC",
                    msg->robot_name.c_str(), msg->battery_level, msg->temperature);

        if (msg->battery_level < 20.0) {
            RCLCPP_WARN(this->get_logger(), "[%s] Low battery: %.1f%%",
                        msg->robot_name.c_str(), msg->battery_level);
        }

        if (msg->temperature > 75.0) {
            RCLCPP_WARN(this->get_logger(), "[%s] High temperature: %.1fC",
                        msg->robot_name.c_str(), msg->temperature);
        }
    }

    rclcpp::Subscription<health_interfaces::msg::RobotHealth>::SharedPtr subscription_;
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HealthMonitor>());
    rclcpp::shutdown();
    return 0;
}