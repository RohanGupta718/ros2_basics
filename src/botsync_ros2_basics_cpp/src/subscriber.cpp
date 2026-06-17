#include "rclcpp/rclcpp.hpp" // Included for overarching ROS 2 C++ capabilities
#include "std_msgs/msg/string.hpp"
#include <functional>
#include <memory>

class BotsyncSubscriber : public rclcpp::Node{
    public:
        BotsyncSubscriber() : Node("botsync_cpp_subscriber") {
            subscription_ = this->create_subscription<std_msgs::msg::String>(
                "botsync_cpp_topic", 10, std::bind(&BotsyncSubscriber::messageCallback, this, std::placeholders::_1)
            );
            RCLCPP_INFO(this->get_logger(), "Subscriber node has been started.");
        }
    private:
        void messageCallback(const std::shared_ptr<const std_msgs::msg::String> & msg) {
            RCLCPP_INFO(this->get_logger(), "Received: '%s'", msg->data.c_str());
        }
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BotsyncSubscriber>());
    rclcpp::shutdown();
    return 0;
}