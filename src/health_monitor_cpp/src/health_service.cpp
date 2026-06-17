#include "rclcpp/rclcpp.hpp"
#include "health_interfaces/msg/robot_health.hpp"
#include "health_interfaces/srv/get_battery_status.hpp"
#include <functional>
#include <memory>

using GetBatteryStatus = health_interfaces::srv::GetBatteryStatus;

class HealthService : public rclcpp::Node {
public:
    HealthService() : Node("health_service") {
        subscription_ = this->create_subscription<health_interfaces::msg::RobotHealth>(
            "robot_health",
            10,
            std::bind(&HealthService::healthCallback, this, std::placeholders::_1)
        );

        service_ = this->create_service<GetBatteryStatus>(
            "get_battery_status",
            std::bind(&HealthService::handleRequest, this, std::placeholders::_1, std::placeholders::_2)
        );

        RCLCPP_INFO(this->get_logger(), "Health Service has been started.");
    }

private:
    void healthCallback(const health_interfaces::msg::RobotHealth::SharedPtr msg) {
        latest_battery_level_ = msg->battery_level;
        latest_temperature_ = msg->temperature;
        has_data_ = true;
    }

    void handleRequest(
        const std::shared_ptr<GetBatteryStatus::Request> request,
        std::shared_ptr<GetBatteryStatus::Response> response)
    {
        (void)request; // request has no fields, nothing to read

        if (!has_data_) {
            RCLCPP_WARN(this->get_logger(), "Battery status requested before any data was received.");
            response->battery_level = 0.0;
            response->is_healthy = false;
            return;
        }

        response->battery_level = latest_battery_level_;
        response->is_healthy = (latest_battery_level_ >= 20.0) && (latest_temperature_ <= 75.0);

        RCLCPP_INFO(this->get_logger(), "Battery status requested: battery=%.1f healthy=%s",
                    response->battery_level, response->is_healthy ? "true" : "false");
    }

    rclcpp::Subscription<health_interfaces::msg::RobotHealth>::SharedPtr subscription_;
    rclcpp::Service<GetBatteryStatus>::SharedPtr service_;

    double latest_battery_level_{0.0};
    double latest_temperature_{0.0};
    bool has_data_{false};
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<HealthService>());
    rclcpp::shutdown();
    return 0;
}