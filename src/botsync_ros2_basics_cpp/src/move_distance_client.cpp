#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "botsync_ros2_interfaces/action/move_distance.hpp"
#include <chrono>
#include <memory>

using namespace std::chrono_literals;

class MoveDistanceClient : public rclcpp::Node
{
public:
    using MoveDistance = botsync_ros2_interfaces::action::MoveDistance;
    using GoalHandleMoveDistance = rclcpp_action::ClientGoalHandle<MoveDistance>;

    MoveDistanceClient() : Node("move_distance_client")
    {
        client_ = rclcpp_action::create_client<MoveDistance>(this, "move_distance");
    }

    void send_goal(float target_distance, float speed)
    {
        if (!client_->wait_for_action_server(5s)) {
            RCLCPP_ERROR(this->get_logger(), "Action server not available.");
            rclcpp::shutdown();
            return;
        }

        MoveDistance::Goal goal_msg;
        goal_msg.target_distance = target_distance;
        goal_msg.speed = speed;

        auto options = rclcpp_action::Client<MoveDistance>::SendGoalOptions();

        options.goal_response_callback =
            [this](GoalHandleMoveDistance::SharedPtr goal_handle) {
                if (!goal_handle) {
                    RCLCPP_ERROR(this->get_logger(), "Goal was rejected by the server.");
                } else {
                    RCLCPP_INFO(this->get_logger(), "Goal accepted by the server.");
                }
            };

        options.feedback_callback =
            [this](GoalHandleMoveDistance::SharedPtr,
                const std::shared_ptr<const MoveDistance::Feedback> feedback) {
                RCLCPP_INFO(this->get_logger(), "Distance: %.2f m | Progress: %.1f%%",
                    feedback->current_distance,
                    feedback->percent_complete);
            };

        options.result_callback =
            [this](const GoalHandleMoveDistance::WrappedResult & result) {
                switch (result.code) {
                    case rclcpp_action::ResultCode::SUCCEEDED:
                        RCLCPP_INFO(this->get_logger(),
                            "Action succeeded: %.2f m | %s",
                            result.result->final_distance,
                            result.result->message.c_str());
                        break;
                    case rclcpp_action::ResultCode::ABORTED:
                        RCLCPP_ERROR(this->get_logger(), "Action aborted.");
                        break;
                    case rclcpp_action::ResultCode::CANCELED:
                        RCLCPP_WARN(this->get_logger(), "Action canceled.");
                        break;
                    default:
                        RCLCPP_ERROR(this->get_logger(), "Unknown result code.");
                        break;
                }
                rclcpp::shutdown();
            };

        client_->async_send_goal(goal_msg, options);
    }

private:
    rclcpp_action::Client<MoveDistance>::SharedPtr client_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MoveDistanceClient>();
    node->send_goal(2.5f, 0.5f);
    rclcpp::spin(node);
    return 0;
}
