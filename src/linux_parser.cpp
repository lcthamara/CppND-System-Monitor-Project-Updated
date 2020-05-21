#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, version, kernel;
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
float LinuxParser::MemoryUtilization() { string line;
  string key;
  string value;
  string memtot;
  string memfree;
  std::ifstream memorystream(kProcDirectory + kMeminfoFilename);
  if (memorystream.is_open()) {
    while (std::getline(memorystream, line)) {
      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memtot = value;
        }
        if (key == "MemFree:") {
          memfree = value;
        }
      }
      
    }
  }
  memorystream.close();
  return (stof(memtot) - stof(memfree)) / stof(memtot);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string value1;
  string value2;
  std::ifstream uptimestream(kProcDirectory + kUptimeFilename);
  if (uptimestream.is_open()) {
    std::getline(uptimestream, line);
    std::istringstream linestream(line);
    while (linestream >> value1 >> value2) {
      uptimestream.close();
      return std::stol(value1); 
    }
  }
  uptimestream.close(); 
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line;
  string cputotal;
  string v1, v2, v3, v4;
  std::ifstream jiffiesstream(kProcDirectory + kStatFilename);
  if (jiffiesstream.is_open()) {
    std::getline(jiffiesstream, line);
    std::istringstream linestream(line);
    while (linestream >> cputotal >> v1 >> v2 >> v3 >> v4) {
      jiffiesstream.close();
      return std::stol(v1) + std::stol(v2) + std::stol(v3) + std::stol(v4); 
    }
  }
  jiffiesstream.close();
  return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string  cputotal;
  string v1, v2, v3, v4;
  std::ifstream activejiffiesstream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (activejiffiesstream.is_open()) {
    std::getline(activejiffiesstream, line);
    std::istringstream linestream(line);
    while (linestream >> cputotal >> v1 >> v2 >> v3 >> v4) {
      activejiffiesstream.close();
      return std::stol(v1) + std::stol(v2) + std::stol(v3); 
    }
  }
  activejiffiesstream.close();
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line;
  string cputotal;
  string v1, v2, v3, v4;
  std::ifstream activeJiffiesStream(kProcDirectory + kStatFilename);
  if (activeJiffiesStream.is_open()) {
    std::getline(activeJiffiesStream, line);
    std::istringstream linestream(line);
    while (linestream >> cputotal >> v1 >> v2 >> v3 >> v4) {
      activeJiffiesStream.close();
      return std::stol(v1) + std::stol(v2) + std::stol(v3); 
    }
  }
  activeJiffiesStream.close();
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line;
  string cputotal;
  string v1, v2, v3, v4;
  std::ifstream idlejiffiesstream(kProcDirectory + kStatFilename);
  if (idlejiffiesstream.is_open()) {
    std::getline(idlejiffiesstream, line);
    std::istringstream linestream(line);
    while (linestream >> cputotal >> v1 >> v2 >> v3 >> v4) {
      idlejiffiesstream.close();
      return std::stol(v4); 
    }
  }
  idlejiffiesstream.close();
  return 0;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream totalProcStream(kProcDirectory + kStatFilename);
  if (totalProcStream.is_open()) {
    while (std::getline(totalProcStream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          totalProcStream.close();
          return stoi(value);
        }
      }
    }
  }
  totalProcStream.close();
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream runningstream(kProcDirectory + kStatFilename);
  if (runningstream.is_open()) {
    while (std::getline(runningstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          runningstream.close();
          return stoi(value);
        }
      }
    }
  }
  runningstream.close();
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream commandstream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (commandstream.is_open()) {
    std::getline(commandstream, line);
    std::istringstream linestream(line);
    commandstream.close();
    return line;    
  }
  commandstream.close();
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  int ram;
  std::ifstream ramstream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (ramstream.is_open()) {
    while (std::getline(ramstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          ramstream.close();
          ram = std::stof(value);
          ram = int(ram / 1024); //convert kB to MB
          value = to_string(ram);
          return value;
        }
      }
    }
  }
  ramstream.close();
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream uidstream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (uidstream.is_open()) {
    while (std::getline(uidstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uidstream.close();
          return value;
        }
      }
    }
  }
  uidstream.close();
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string key1, key2, key3;
  string uid;
  uid = LinuxParser::Uid(pid);
  std::ifstream userstream(kPasswordPath);
  if (userstream.is_open()) {
    while (std::getline(userstream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key1 >> key2 >> key3) {
        if (key3 == uid) {
          userstream.close();
          return key1;
        }
      }
    }
  }
  userstream.close();
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string uptime;
  std::ifstream upTIMEstream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (upTIMEstream.is_open()) {
    while (std::getline(upTIMEstream, line)) {
      std::istringstream linestream(line);
      for (int i = 0; i< 22; i++) {
        linestream >> uptime;
      }
      upTIMEstream.close() ;
      return stol(uptime) / sysconf(_SC_CLK_TCK);     
    }
  }
  upTIMEstream.close();
  return 0;
}

std::vector<float> LinuxParser::threeVariables(int pid) {
  string line;
  string var;
  float utime, stime, starttime;
  std::vector<float> threeVar;
  std::ifstream threeVARstream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (threeVARstream.is_open()) {
    std::getline(threeVARstream, line);
    std::istringstream linestream(line);
    for (int i = 0; i< 22; i++) {
      linestream >> var;
      if (i==13) {
        utime = stol(var);
      }
      if (i==14){
        stime = stol(var);
      }
      if (i==21) {
        starttime = stol(var);
      }
    } 
  }
  threeVar = {utime, stime, starttime};
  threeVARstream.close();
  return threeVar;
}