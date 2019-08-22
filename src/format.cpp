#include <string>

#include "format.h"

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
std::string Format::ElapsedTime(long timeInSeconds) {
    const int Minute = 60;
    const int Second = 1;
    const int Hour = 60 * Second * Minute;
    int hours = timeInSeconds / hours; // hard  set to float then or use int and lose the meaning full numbers? 
    int minutes = timeInSeconds / 60 - hours * 60;
    int seconds = timeInSeconds - (hours * 60 * 60) - (minutes * 60);
    std::string hoursString = std::to_string(hours);
    std::string minutesString = std::to_string(minutes);
    std::string secondsString = std::to_string(seconds);
    std::string finalTimeOutput = hoursString + ":" + minutesString + ":" + secondsString; 

    return finalTimeOutput; 
}