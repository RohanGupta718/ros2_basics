#include "rclcpp/rclcpp.hpp"
#include "convoy_interfaces/msg/convoy_position.hpp"
#include "convoy_interfaces/srv/convoy_status.hpp"
#include <functional>
#include <memory>
#include <cmath>

class ConvoyService : public rclcpp::Node
{
public:
    ConvoyService() : Node("convoy_service")
    {
        leader_subscription_ = this->create_subscription<convoy_interfaces::msg::ConvoyPosition>(
            "leader/position", 10, std::bind(&ConvoyService::lead_callback, this, std::placeholders::_1)
        );
        follower_subscription_ = this->create_subscription<convoy_interfaces::msg::ConvoyPosition>(
            "follower/position", 10, std::bind(&ConvoyService::follow_callback, this, std::placeholders::_1)
        );
        service_ = this->create_service<convoy_interfaces::srv::ConvoyStatus>(
            "convoy/status", std::bind(&ConvoyService::status_callback, this, std::placeholders::_1, std::placeholders::_2)
        );

        RCLCPP_INFO(this->get_logger(), "Convoy Service Node has been started.");
    }

private:
    void lead_callback(const convoy_interfaces::msg::ConvoyPosition::SharedPtr lead_msg) {
        leader_x_ = lead_msg->x_coord;
        RCLCPP_INFO(this->get_logger(), "Received leader position: x=%.2f, y=%.2f", lead_msg->x_coord, lead_msg->y_coord);
    }

    void follow_callback(const convoy_interfaces::msg::ConvoyPosition::SharedPtr follow_msg) {
        follower_x_ = follow_msg->x_coord;
        RCLCPP_INFO(this->get_logger(), "Received follower position: x=%.2f, y=%.2f", follow_msg->x_coord, follow_msg->y_coord);
    }

    void status_callback(const std::shared_ptr<convoy_interfaces::srv::ConvoyStatus::Request> request,
    std::shared_ptr<convoy_interfaces::srv::ConvoyStatus::Response> response) {
        response->leader_x = leader_x_;
        response->follower_x = follower_x_;
        response->gap = std::abs(leader_x_ - follower_x_);
        response->is_healthy = (response->gap < 3.0); 
    }

    float leader_x_ = 0.0;
    float follower_x_ = 0.0;
    rclcpp::Subscription<convoy_interfaces::msg::ConvoyPosition>::SharedPtr leader_subscription_;
    rclcpp::Subscription<convoy_interfaces::msg::ConvoyPosition>::SharedPtr follower_subscription_;
    rclcpp::Service<convoy_interfaces::srv::ConvoyStatus>::SharedPtr service_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ConvoyService>());
    rclcpp::shutdown();
    return 0;
}