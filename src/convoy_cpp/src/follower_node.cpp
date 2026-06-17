#include "rclcpp/rclcpp.hpp"
#include "convoy_interfaces/msg/convoy_position.hpp"
#include <chrono>
#include <functional>
#include <memory>

using namespace std::chrono_literals;

class FollowerNode : public rclcpp::Node
{
    public:    
        FollowerNode() : Node("follower_node") {
            subscription_ = this->create_subscription<convoy_interfaces::msg::ConvoyPosition>("leader/position", 10, std::bind(&FollowerNode::position_callback, this, std::placeholders::_1));
            publisher_ = this->create_publisher<convoy_interfaces::msg::ConvoyPosition>("follower/position", 10);
            RCLCPP_INFO(this->get_logger(), "Follower node has been started.");
        }

    private:
        void position_callback(const convoy_interfaces::msg::ConvoyPosition::SharedPtr msg) {
            auto follower_msg = convoy_interfaces::msg::ConvoyPosition();
            follower_msg.x_coord = msg->x_coord - 2.0; 
            follower_msg.y_coord = msg->y_coord;
            follower_msg.speed = msg->speed;
            follower_msg.leader_name = msg->leader_name;            

            float gap = msg->x_coord - follower_msg.x_coord;
            if (gap > 3.0) {
                RCLCPP_WARN(this->get_logger(), "Gap between too large: %.2f", gap);
            }

            publisher_->publish(follower_msg);
            RCLCPP_INFO(this->get_logger(), "Published follower position: x=%.2f, y=%.2f | Gap=%.2f", follower_msg.x_coord, follower_msg.y_coord, gap);
        }

        rclcpp::Subscription<convoy_interfaces::msg::ConvoyPosition>::SharedPtr subscription_;
        rclcpp::Publisher<convoy_interfaces::msg::ConvoyPosition>::SharedPtr publisher_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FollowerNode>());
    rclcpp::shutdown();
    return 0;
}