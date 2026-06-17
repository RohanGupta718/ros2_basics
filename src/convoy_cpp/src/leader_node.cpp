#include "rclcpp/rclcpp.hpp"
#include "convoy_interfaces/msg/convoy_position.hpp"
#include <chrono>
#include <functional>
#include <memory>

using namespace std::chrono_literals;

class LeaderNode : public rclcpp::Node {
    public:
        LeaderNode() : Node("leader_node") {
            this->declare_parameter<std::string>("leader_name", "Leader");
            leader_name_ = this->get_parameter("leader_name").as_string();
            publisher_ = this->create_publisher<convoy_interfaces::msg::ConvoyPosition>("leader/position", 10);
            timer_ = this->create_wall_timer(500ms, std::bind(&LeaderNode::publish_position, this));
            RCLCPP_INFO(this->get_logger(), "Leader node started with name: %s", leader_name_.c_str());
        }
        
        

    private:
        void publish_position() {
            auto message = convoy_interfaces::msg::ConvoyPosition();
            message.leader_name = leader_name_;
            message.x_coord = x_position_;
            message.y_coord = y_position_;
            message.speed = speed_;
            publisher_->publish(message);

            RCLCPP_INFO(this->get_logger(), "Published position: x=%f, y=%f, speed=%f", x_position_, y_position_, speed_);
            x_position_ += speed_;
        }

        float y_position_ = 0.0;
        float x_position_ = 0.0;
        float speed_ = 0.5;
        rclcpp::Publisher<convoy_interfaces::msg::ConvoyPosition>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        std::string leader_name_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LeaderNode>());
    rclcpp::shutdown();
    return 0;
}