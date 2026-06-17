#include "rclcpp/rclcpp.hpp"
#include "botsync_ros2_interfaces/srv/botsync_calc.hpp"
#include <chrono>
#include <future>
#include <memory>
#include <string>

using namespace std::chrono_literals;

class BotsyncCalcClient : public rclcpp::Node
{
public:
    BotsyncCalcClient() : Node("botsync_calc_client")
    {
        client_ = this->create_client<botsync_ros2_interfaces::srv::BotsyncCalc>(
            "botsync_calc");
    }

    bool waitForServer(std::chrono::seconds timeout = 5s)
    {
        if (!client_->wait_for_service(timeout)) {
            RCLCPP_ERROR(this->get_logger(), "Service not available!");
            return false;
        }
        return true;
    }

    std::shared_future<botsync_ros2_interfaces::srv::BotsyncCalc::Response::SharedPtr>sendRequest(double a, double b, const std::string & operation) {
        auto request = std::make_shared<botsync_ros2_interfaces::srv::BotsyncCalc::Request>();
        request->a = a;
        request->b = b;
        request->operation = operation;
        RCLCPP_INFO(this->get_logger(), "Sending: %.2f %s %.2f", a, operation.c_str(), b);
        return client_->async_send_request(request).future.share();
    }
private:
    rclcpp::Client<botsync_ros2_interfaces::srv::BotsyncCalc>::SharedPtr client_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto client_node = std::make_shared<BotsyncCalcClient>();

    if (!client_node->waitForServer()) {
        rclcpp::shutdown();
        return 1;
    }

    auto future = client_node->sendRequest(25.0, 5.0, "divide");
    if (rclcpp::spin_until_future_complete(client_node, future) == rclcpp::FutureReturnCode::SUCCESS) {
        auto response = future.get();
        RCLCPP_INFO(client_node->get_logger(), "Result: %.2f | Success: %s | %s", response->result, response->success ? "true" : "false", response->message.c_str());
    } else {
        RCLCPP_ERROR(client_node->get_logger(), "Failed to call service");
    }

    rclcpp::shutdown();
    return 0;
}
