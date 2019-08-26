#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include "format.h"
#include "linux_parser.h"
#include <iostream>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

const std::string pathToVersionInLinux = "/prov/version";

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
   /** for now implementation will send back total used memory
    re review https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
    for further information and calculation input
   **/
   std::ifstream myFile;
   std::string line,sectionHeader, capcityOfMemory, memoryData, content;
   std::string memTotal("MemTotal:"), memFree("MemFree:");
   std::vector<double> dataVector;
   double amountOfMemory(0);

   myFile.open(kProcDirectory + kMeminfoFilename);
   if (myFile.is_open()) {
      while(std::getline(myFile, line)) {
        std::istringstream stream(line);
        stream >> sectionHeader >>  memoryData >> capcityOfMemory;
        // right now this type of writing will assume that the first one is memTotal
        // and the second is memfree consider a structure for saving them or a better way to implement it
        // std::cout << sectionHeader << "\n";
        if(sectionHeader == memTotal || sectionHeader == memFree) {
          dataVector.push_back(std::stod(memoryData));
        }
        // create new if that checks if both strings are kb or same value of storage
      }     
   }
   
   if(dataVector.size() == 2) {
     amountOfMemory = dataVector[0] - dataVector[1];
   }

   return amountOfMemory; 
   }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream myFile;
  std:string line;
  std::string uptime;
  myFile.open(kProcDirectory + kUpTime);
  std::string content;
  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
      std::istringstream stream(line);
      stream >> uptime;
      std::cout << "uptime is: " << uptime << "\n";
    }
  }
  long int uptimeLongInt = std::stoi(uptime);
  // toDo: test design and check if linux parser uptime really need to retunr long int or just a parsed string
  // might just be easier.
  return uptimeLongInt; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) {
 return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

int main() {
  std::cout <<  LinuxParser::MemoryUtilization();
  
  return 0;
}