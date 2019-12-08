#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  	auto myVector = LinuxParser::CpuUtilization();
  	int user = std::stoi(myVector[0]);
  	int nice = std::stoi(myVector[1]);
  	int system = std::stoi(myVector[2]);
  	int  idle = std:: stoi(myVector[3]);
  	int ioWait = std::stoi(myVector[4]);
    int irq = std::stoi(myVector[5]);
  	int softIrq = std::stoi(myVector[6]);
  	int steal = std:: stoi(myVector[7]);
  	int guest = std::stoi(myVector[8]);
    int guest_nice = std::stoi(myVector[9]);
  	
  	int userTime = user - guest;
  	int niceTime = nice - guest_nice;
  	int idlealltime = idle - ioWait;
  	int systemalltime = system + irq + softIrq;
    int virtualTime = guest + guest_nice;
  	int totalTime  = userTime + niceTime + systemalltime + idlealltime + steal + virtualTime;	
  
 	return (totalTime - idlealltime) / totalTime; // value should be 0.xx find out how to do that
}