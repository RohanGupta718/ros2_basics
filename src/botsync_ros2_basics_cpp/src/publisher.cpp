#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <string>

class BotsyncPublisher : public rclcpp::Node {
    public:
        BotsyncPublisher() : Node("botsync_cpp_publisher"), count_(0) {
            publisher_ = this->create_publisher<std_msgs::msg::String>("botsync_cpp_topic", 10);
            timer_ = this->create_wall_timer(
                std::chrono::milliseconds(500),
                std::bind(&BotsyncPublisher::publishMessage, this)
            );
            RCLCPP_INFO(this->get_logger(), "Publisher node has been started.");
        }

    private:
        void publishMessage() {
            auto msg = std_msgs::msg::String();
            msg.data = "Hello, Botsync! " + std::to_string(count_++);
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msg.data.c_str());
            publisher_->publish(msg);
        }

        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        size_t count_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BotsyncPublisher>());
    rclcpp::shutdown();
    return 0;
}