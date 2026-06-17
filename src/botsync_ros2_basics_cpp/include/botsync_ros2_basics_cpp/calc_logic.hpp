#pragma once
#include <string>
namespace botsync_ros2_basics_cpp
{
struct CalcResult { double result; bool success; std::string message; };

inline CalcResult perform_operation(double a, double b, const std::string & operation)
{
    if (operation == "add") return {a + b, true, "Addition successful"};
    if (operation == "subtract") return {a - b, true, "Subtraction successful"};
    if (operation == "multiply") return {a * b, true, "Multiplication successful"};
    if (operation == "divide") {
        if (b == 0.0) return {0.0, false, "ERROR: Division by zero!"};
        return {a / b, true, "Division successful"};
    }
    return {0.0, false, "Unknown operation: " + operation};
}
}