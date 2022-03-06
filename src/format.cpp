#include <string>

#include "format.h"

using std::string;
const int ONE_HOUR = 60 * 60;
const int ONE_MINUE = 60;
// : Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hour = seconds / ONE_HOUR;
    seconds = seconds % ONE_HOUR;
    long min = seconds / ONE_MINUE;
    seconds = seconds % ONE_MINUE;
    long sec = seconds;
    return std::to_string(hour) + ":" + std::to_string(min) + ":" +
           std::to_string(sec);
}