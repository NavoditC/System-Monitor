#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int p)
{
  pid = p;
  cpu_utilization = Process::CpuUtilization();
}

// Return this process's ID
int Process::Pid() { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() {
  string line,s;
  int i;
  float utime,stime,cutime,cstime,cpu_usage,uptime;
  std::ifstream stream(LinuxParser::kProcDirectory+to_string(pid)+LinuxParser::kStatFilename);
  if(stream.is_open())
  {
    getline(stream,line);
    std::istringstream linestream(line);
    for(i=1;i<=13;i++)
      linestream >> s;
    linestream >> utime >> stime >> cutime >> cstime;
    cpu_usage = (utime + stime + cutime + cstime)/sysconf(_SC_CLK_TCK);
    uptime = static_cast<float>(Process::UpTime());
    return cpu_usage/uptime;
  }
  else
  {
    return 0.0;
  }
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// Return the age of this process (in seconds)
long Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return (a.cpu_utilization<this->cpu_utilization);

}