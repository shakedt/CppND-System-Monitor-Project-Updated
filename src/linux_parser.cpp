#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include "format.h"
#include "linux_parser.h"
#include <iostream>
#include <vector>
#include <unistd.h>

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
   float totalMemory(0), freeMemory(0), amountOfMemory(0);

   myFile.open(kProcDirectory + kMeminfoFilename);
   if (myFile.is_open()) {
      while(std::getline(myFile, line)) {
        std::istringstream stream(line);
        stream >> sectionHeader >>  memoryData >> capcityOfMemory;
          std::istringstream convert(memoryData);
          if(sectionHeader == memTotal) {
            convert >> totalMemory;
          } 
          
          if(sectionHeader == memFree) {
            convert >> freeMemory;
          }
      }     
   }

   amountOfMemory = (totalMemory - freeMemory) / totalMemory;
   return amountOfMemory; 
   }

// Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream myFile;
  std::string line;
  std::string uptime;
  myFile.open(kProcDirectory + kUpTime);

  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
      std::istringstream stream(line);
      stream >> uptime;
    }
  }
  long int uptimeLongInt = std::stoi(uptime);
  // toDo: test design and check if linux parser uptime really need to retunr long int or just a parsed string
  // might just be easier.
  return uptimeLongInt; 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  std::ifstream myFile;
  std::string line, data, cpuData;
  int totalSystemTicsUsed = 0;
  const std::string cpu = "cpu";
  // ToDo: recheck logic is somthing wrong with the concept here ? do i need to split with sys conf item ? 
  myFile.open(kProcDirectory + kStatFilename);
  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
      std::istringstream stream(line);
      stream >> data;
      if(data == cpu) {
        stream >>  cpuData; 
        totalSystemTicsUsed = std::stoi(cpuData);
        
      }
    }
  }
  return totalSystemTicsUsed;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  std::ifstream myFile;
  std::string line;
  std::string time;
  const int utimeLocation(14), stimeLocation(15);
  int counter(0), sum = 0;

  myFile.open(kProcDirectory + std::to_string(pid) + kStatFilename);

  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
      std::istringstream stream(line);
      while(stream >> time) {
        if(counter == utimeLocation || counter == stimeLocation) {
         sum += std::stoi(time) / sysconf(_SC_CLK_TCK);
        }
        counter++;        
      } 
    }    
  }
  return sum;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream myFile;
  std::string line;
  std::string data;
  int totalCpuTime(0), idleCpuTime(0);
  int activeTime(0);
  const std::string cpuName;  
  myFile.open(kProcDirectory + kStatFilename);

  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
      std::istringstream stream(line);
      std::cout << "again ";      
      stream >> data;
      if(data == cpuName) { 
        stream >> totalCpuTime;
        stream >> idleCpuTime >> idleCpuTime >> idleCpuTime;

        activeTime = std::abs(totalCpuTime - idleCpuTime);
      }
    }
  }
  return activeTime;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream myFile;
  std::string line, idleJiffies;
  std::string cpuName = "cpu";
  int idleCpuTime(0); // aka jiffiys
  myFile.open(kProcDirectory + kStatFilename);

  if(myFile.is_open()) {
    while(std::getline(myFile, line))  {
      std::istringstream stream(line);

      stream >> idleJiffies;
      if(idleJiffies == cpuName) {
        stream >> idleJiffies >> idleJiffies >> idleJiffies >> idleJiffies;
        idleCpuTime = std::stoi(idleJiffies);
      }
    }
  }
  return idleCpuTime;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  // ToDo: change calc to correctly calculate real time utilzation and not all time calc
  std::ifstream myFile;
  std::string line, cpuAllTimeUsage, data;
  const std::string cpu = "cpu";
  std::vector<string> cpuUtilization;
  myFile.open(kProcDirectory + kStatFilename);
  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
     std::istringstream stream(line);

     stream >> cpuAllTimeUsage;
     if(cpuAllTimeUsage == cpu) {
       while(stream >> data) {
        cpuUtilization.push_back(data);
       }
     } 
    }
  }
  return cpuUtilization;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream myFile;
  std::string line, proccesses, proccessesNumber;
  std::string totalProcessesName = "processes";
  int totalProccessNumber;
  myFile.open(kProcDirectory + kStatFilename);
  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
       std::istringstream stream(line);
       stream >> proccesses >> proccessesNumber;
       if(proccesses == totalProcessesName) {
        totalProccessNumber = std::stoi(proccessesNumber, nullptr, 0); 
       }
    }
  }

  return totalProccessNumber; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream myFile;
  std::string line, proccesses, proccessesNumber;
  std::string runningProccessesName = "procs_running";
  int runingProccesses(0);

  myFile.open(kProcDirectory + kStatFilename);

  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
      std::istringstream stream(line);
      stream >> proccesses >> proccessesNumber;
      
      if(proccesses == runningProccessesName) {
        runingProccesses = std::stoi(proccessesNumber, nullptr, 0);
      }
    }
  }

  return runingProccesses;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream myFile;
  std::string line, command;

  myFile.open(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
      std::istringstream stream(line);

      stream >> command;
    }
  }
  return command; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
 std::ifstream myFile;
 std::string line, ramSizeString, ramSizeIntAsString;
 const std::string ramSize = "VmSize:";
 std::string turnRamSizeFromKbToMb;
 
 myFile.open(kProcDirectory + std::to_string(pid) + kStatusFilename);

 if(myFile.is_open()) {
   while(std::getline(myFile, line)) {
     std::stringstream stream(line);
     
     stream >> ramSizeString >> ramSizeIntAsString;

     if(ramSizeString == ramSize) {
       break;
     }
   }
 }
 turnRamSizeFromKbToMb = std::to_string(std::stoi(ramSizeIntAsString) / 1000);

 return turnRamSizeFromKbToMb; 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::ifstream myFile;
  std::string line, userIdString, userIdNumber;
  std::string returnUserIdValue = "Nothing Found";
  const std::string userId = "Uid:";
  
  myFile.open(kProcDirectory + std::to_string(pid) + kStatusFilename);
  
  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
      std::istringstream stream(line);

      stream >> userIdString >> userIdNumber;

      if(userId == userIdString) {
       returnUserIdValue = userIdNumber; 
      }
    }
  }
  
  return returnUserIdValue; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::string proccessUserId = LinuxParser::Uid(pid);
  std::ifstream myFile;
  std::string line, userName;
  std::string userNameToReturn;

  myFile.open(kPasswordPath);

  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
      std::istringstream stream(line);

      stream >> userName;

      std::size_t found = userName.find(proccessUserId);
      if(found != std::string::npos) {
        found = userName.find(":");
      
        userNameToReturn = userName.substr(0, found);
        break;
      }
    }
  }

  return userNameToReturn;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::ifstream myFile;
  std::string line, data;
  int counter(0);
  const int locationOfMemoryUpTime = 22;
  myFile.open(kProcDirectory + std::to_string(pid) + kStatFilename);
  int timeToSeconds;

  if(myFile.is_open()) {
    while(std::getline(myFile, line)) {
      std::istringstream stream(line);
      while(stream >> data) {
        counter++;
        if(counter == locationOfMemoryUpTime) {
          break;
        }
      }
    }
  }

  timeToSeconds = std::stoi(data) / sysconf(_SC_CLK_TCK);
  
  return timeToSeconds; 
}