#include "rclcpp/rclcpp.hpp"
#include <functional>
#include <memory>
#include "botsync_ros2_interfaces/srv/botsync_calc.hpp"

class BotsyncCalcServer : public rclcpp::Node
{
public:
    BotsyncCalcServer() : Node("botsync_calc_server")
    {
        service_ = this->create_service<botsync_ros2_interfaces::srv::BotsyncCalc>(
            "botsync_calc",
            std::bind(&BotsyncCalcServer::handleRequest, this,
                      std::placeholders::_1, std::placeholders::_2)
        );

        RCLCPP_INFO(this->get_logger(), "Botsync Calculator Service Server is ready.");
    }

private:
    void handleRequest(
        const std::shared_ptr<botsync_ros2_interfaces::srv::BotsyncCalc::Request> request,
        std::shared_ptr<botsync_ros2_interfaces::srv::BotsyncCalc::Response> response)
    {
        RCLCPP_INFO(this->get_logger(), "Received: %.2f %s %.2f",
                    request->a, request->operation.c_str(), request->b);

            response ->success = true;

        if (request->operation == "add") {
            response->result = request->a + request->b;
            response->message = "Addition performed successfully.";
        } else if (request->operation == "subtract") {
            response->result = request->a - request->b;
            response->message = "Subtraction performed successfully.";
        } else if (request->operation == "multiply") {
            response->result = request->a * request->b;
            response->message = "Multiplication performed successfully.";
        } else if (request->operation == "divide") {
            if (request->b == 0.0) {
                response->success = false;
                response->message = "Error: Division by zero is not allowed.";
                response->result = 0.0; // Default value for error case
            } else {
                response->result = request->a / request->b;
                response->message = "Division performed successfully.";
            }
        } else {
            response->success = false;
            response->message = "Unknown Operation" + request->operation;
        }

        RCLCPP_INFO(this->get_logger(), "Result: %.2f | %s", response->result, response->message.c_str());
    }

    rclcpp::Service<botsync_ros2_interfaces::srv::BotsyncCalc>::SharedPtr service_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BotsyncCalcServer>());
    rclcpp::shutdown();
    return 0;
}