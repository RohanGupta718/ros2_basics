#include "rclcpp/rclcpp.hpp"
#include "health_interfaces/msg/robot_health.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <random>

using namespace std::chrono_literals;

class HealthPublisher : public rclcpp::Node {
public:
    HealthPublisher() : Node("health_publisher") {
        this->declare_parameter<std::string>("robot_name", "DefaultBot");
        robot_name_ = this->get_parameter("robot_name").as_string();

        publisher_ = this->create_publisher<health_interfaces::msg::RobotHealth>("robot_health", 10);

        timer_ = this->create_wall_timer(
            1s,
            std::bind(&HealthPublisher::publishHealthStatus, this)
        );

        RCLCPP_INFO(this->get_logger(), "Health Publisher for '%s' has been started.", robot_name_.c_str());
    }

private:
    void publishHealthStatus() {
        auto message = health_interfaces::msg::RobotHealth();

        message.header.stamp = this->get_clock()->now();
        message.robot_name = robot_name_;

        battery_level_ = std::max(0.0, battery_level_ - 0.5);
        message.battery_level = battery_level_;

        message.temperature = temp_dist_(gen_);

        RCLCPP_DEBUG(this->get_logger(), "Publishing: battery=%.1f temp=%.1f",
                     message.battery_level, message.temperature);

        publisher_->publish(message);
    }

    rclcpp::Publisher<health_interfaces::msg::RobotHealth>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::string robot_name_;
    double battery_level_{100.0};

    std::random_device rd_;
    std::mt19937 gen_{rd_()};
    std::uniform_real_distribution<double> temp_dist_{60.0, 80.0};
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HealthPublisher>());
    rclcpp::shutdown();
    return 0;
}