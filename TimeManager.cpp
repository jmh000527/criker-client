#include "TimeManager.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

std::string TimeManager::getCurrentTime() {
	const auto now = std::chrono::system_clock::now();
    const auto time_point = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_point), "%Y-%m-%d %H:%M:%S");

    return oss.str();
}