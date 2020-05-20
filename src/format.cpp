#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int sec, min, hour;
    string timeOutput;
    hour = int (seconds / 3600);
    min = seconds - hour*3600;
    min = int(min / 60);
    sec = seconds - min*60 - hour*3600;
    if (hour < 10) {
        timeOutput = "0";
    }
    timeOutput += std::to_string(hour) + ":" ;
    if (min < 10) {
        timeOutput += "0";
    }
    timeOutput += std::to_string(min) + ":" ;
    if (sec < 10) {
        timeOutput += "0";
    }
    timeOutput += std::to_string(sec);

    return timeOutput;
}