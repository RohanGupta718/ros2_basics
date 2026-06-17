#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "botsync_ros2_interfaces/action/move_distance.hpp"
#include <chrono>
#include <memory>
#include <thread>

using namespace std::chrono_literals;

class MoveDistanceServer : public rclcpp::Node
{
public:
    using MoveDistance = botsync_ros2_interfaces::action::MoveDistance;
    using GoalHandleMoveDistance = rclcpp_action::ServerGoalHandle<MoveDistance>;

    MoveDistanceServer() : Node("move_distance_server")
    {
        action_server_ = rclcpp_action::create_server<MoveDistance>(
            this,
            "move_distance",
            std::bind(&MoveDistanceServer::handle_goal, this,
                std::placeholders::_1, std::placeholders::_2),
            std::bind(&MoveDistanceServer::handle_cancel, this,
                std::placeholders::_1),
            std::bind(&MoveDistanceServer::handle_accepted, this,
                std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "Move Distance Action Server is ready.");
    }

private:
    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID &,
        std::shared_ptr<const MoveDistance::Goal> goal)
    {
        if (goal->target_distance <= 0.0f || goal->speed <= 0.0f) {
            RCLCPP_WARN(this->get_logger(), "Rejecting invalid goal");
            return rclcpp_action::GoalResponse::REJECT;
        }
        RCLCPP_INFO(this->get_logger(), "Accepted goal: %.2f m at %.2f m/s",
            goal->target_distance, goal->speed);
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandleMoveDistance> goal_handle)
    {
        (void)goal_handle;
        RCLCPP_INFO(this->get_logger(), "Cancel request received");
        return rclcpp_action::CancelResponse::ACCEPT;
    }

    void handle_accepted(const std::shared_ptr<GoalHandleMoveDistance> goal_handle)
    {
        std::thread{std::bind(&MoveDistanceServer::execute, this, goal_handle)}.detach();
    }

    void execute(const std::shared_ptr<GoalHandleMoveDistance> goal_handle)
    {
        const auto goal = goal_handle->get_goal();
        auto feedback = std::make_shared<MoveDistance::Feedback>();
        auto result = std::make_shared<MoveDistance::Result>();

        const int steps = 10;

        for (int i = 1; i <= steps; ++i) {
            if (goal_handle->is_canceling()) {
                result->success = false;
                result->final_distance = feedback->current_distance;
                result->message = "Goal canceled";
                goal_handle->canceled(result);
                return;
            }

            feedback->current_distance =
                goal->target_distance * static_cast<float>(i) / steps;
            feedback->percent_complete =
                100.0f * static_cast<float>(i) / steps;

            goal_handle->publish_feedback(feedback);
            RCLCPP_INFO(this->get_logger(), "Progress: %.1f%%",
                feedback->percent_complete);

            std::this_thread::sleep_for(500ms);
        }

        result->success = true;
        result->final_distance = goal->target_distance;
        result->message = "Target distance reached";
        goal_handle->succeed(result);
    }

    rclcpp_action::Server<MoveDistance>::SharedPtr action_server_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MoveDistanceServer>());
    rclcpp::shutdown();
    return 0;
}
