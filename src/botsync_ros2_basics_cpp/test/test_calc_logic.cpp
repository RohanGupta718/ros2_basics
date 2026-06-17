#include <gtest/gtest.h>
#include "botsync_ros2_basics_cpp/calc_logic.hpp"

TEST(BotsyncCalcLogic, DivideSuccess)
{
    const auto result = botsync_ros2_basics_cpp::perform_operation(25.0, 5.0, "divide");
    EXPECT_TRUE(result.success);
    EXPECT_DOUBLE_EQ(result.result, 5.0);
}

TEST(BotsyncCalcLogic, DivideByZeroFails)
{
    const auto result = botsync_ros2_basics_cpp::perform_operation(25.0, 0.0, "divide");
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.message, "ERROR: Division by zero!");
}