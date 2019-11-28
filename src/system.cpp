#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include<iostream>

#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

using std::cout;
// TODO: Return the system's CPU
Processor& System::Cpu() {
  cpu_ = Processor(); 
  return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  
  for(int pid: pids) {
    Process process(pid);
    processes_.push_back(process);
  }
  return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() {
  return LinuxParser::Kernel();
 }

// Return the system's memory utilization
float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

// Return the operating system name
std::string System::OperatingSystem() {
  return LinuxParser::OperatingSystem();
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses();
}

// Return the total number of processes on the system
int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}

// Return the number of seconds since the system started running
long int System::UpTime() { 
  return LinuxParser::UpTime(); 
}