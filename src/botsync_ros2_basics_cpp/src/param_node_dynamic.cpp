#include "rclcpp/rclcpp.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class DynamicParamNode : public rclcpp::Node
{
public:
    DynamicParamNode() : Node("dynamic_param_node")
    {
        // 1. Declare parameters as usual
        this->declare_parameter<std::string>("robot_name", "BotsyncBot_Dynamic");
        this->declare_parameter<double>("publish_rate_hz", 2.0);

        // 2. Fetch initial values
        robot_name_ = this->get_parameter("robot_name").as_string();
        publish_rate_hz_ = this->get_parameter("publish_rate_hz").as_double();

        // 3. Register the callback handler for runtime dynamic modifications
        param_callback_handle_ = this->add_on_set_parameters_callback(
            std::bind(&DynamicParamNode::parametersCallback, this,
                std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "Started dynamic parameter node.");
        RCLCPP_INFO(this->get_logger(), "Try: ros2 param set /dynamic_param_node robot_name NewName");

        // 4. Initialize timer based on initial parameters
        resetTimer();
    }

private:
    rcl_interfaces::msg::SetParametersResult parametersCallback(
        const std::vector<rclcpp::Parameter> &parameters)
    {
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;
        result.reason = "success";

        for (const auto &param : parameters) {
            if (param.get_name() == "robot_name") {
                robot_name_ = param.as_string();
                RCLCPP_INFO(this->get_logger(),
                    "--> robot_name updated to: %s", robot_name_.c_str());
            }
            else if (param.get_name() == "publish_rate_hz") {
                publish_rate_hz_ = param.as_double();
                RCLCPP_INFO(this->get_logger(),
                    "--> publish_rate_hz updated to: %.1f", publish_rate_hz_);
                resetTimer();
            }
        }

        return result;
    }

    void resetTimer()
    {
        const auto period_ms = std::chrono::milliseconds(
            static_cast<int>(1000.0 / publish_rate_hz_));

        auto new_timer = this->create_wall_timer(
            period_ms,
            std::bind(&DynamicParamNode::timerCallback, this));

        auto old_timer = std::move(timer_);
        timer_ = std::move(new_timer);

        if (old_timer) {
            old_timer->cancel();
            old_timer.reset();
        }
    }

    void timerCallback()
    {
        RCLCPP_INFO(this->get_logger(),
            "[%s] Dynamic Ping @ %.1f Hz", robot_name_.c_str(), publish_rate_hz_);
    }

    std::string robot_name_;
    double publish_rate_hz_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DynamicParamNode>());
    rclcpp::shutdown();
    return 0;
}