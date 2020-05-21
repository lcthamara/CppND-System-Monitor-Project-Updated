#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

void Process::mysetter(int mypid) {
    pid = mypid;
    cpuUtilization = CpuUtilization();
    //theUpTime = LinuxParser::UpTime(pid); 
}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    std::vector<float> myThreeVar = LinuxParser::threeVariables(pid);
    float utime = myThreeVar[0];
    float stime = myThreeVar[1];
    float starttime = myThreeVar[4];

    float uptime = LinuxParser::UpTime();

    float total_time = utime + stime;
    //total_time = total_time + cutime + cstime;
    float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
    float cpu_usage = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds );
    return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return cpuUtilization > a.cpuUtilization;
}