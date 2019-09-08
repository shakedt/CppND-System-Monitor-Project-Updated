#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#ifndef linuxParser
#define linuxParser
#include "linux_parser.h"
#endif

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
// need to do calculations here consider renamind vector to int function on linux parser
// and do calcs there seems to make much more sens
float Process::CpuUtilization() {
    return std::stoi(LinuxParser::CpuUtilization()[0]);
}

// Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid);
}

// Return this process's memory utilization
string Process::Ram() const { 
    return LinuxParser::Ram(pid);
}

// Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(pid); 
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid);
}

// TODO: Overload the "less than" comparison operator for Pocess robjects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return std::stoi(this->Ram()) > std::stoi(a.Ram());
}