#include <string>
#include <iostream>
#include "format.h"

std::string Format::ElapsedTime(long timeInSeconds) {
    const long int MinuteRepresentedInSeconds = 60;
    const long int Second = 1;
    const long int HourRepresentedInSeconds = 60 * Second * MinuteRepresentedInSeconds;
    long int hours = timeInSeconds / HourRepresentedInSeconds; // hard  set to float then or use int and lose the meaning full numbers? 
    long int minutes = (timeInSeconds / MinuteRepresentedInSeconds) - (HourRepresentedInSeconds / MinuteRepresentedInSeconds);
    long int seconds = timeInSeconds - (HourRepresentedInSeconds) - (MinuteRepresentedInSeconds * minutes);
    std::string hoursString = std::to_string(hours);
    std::string minutesString = std::to_string(minutes);
    std::string secondsString = std::to_string(seconds);
    std::string finalTimeOutput = hoursString + ":" + minutesString + ":" + secondsString; 

    return finalTimeOutput; 
}