#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "format.h"
#include <iostream>
#include <iomanip>
using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::getline;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) 
  {
    while (getline(filestream, line)) {
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
    filestream.close();
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string name, quantity, unit, line;
  float memtotal, memfree;
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> name >> quantity >> unit;
      if(name=="MemTotal:")
        memtotal = stof(quantity);
      if(name=="MemFree:"){
        memfree  = stof(quantity);
        break;
      }
    }
    stream.close();
  }
  return (memtotal - memfree)/memtotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  string line;
  float uptime;
  if(stream.is_open())
  {
    getline(stream,line);
    std::istringstream linestream(line);
    linestream >> uptime;
    stream.close();
  }
  return uptime;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory+kStatFilename);
  string s,line;
  int processes;
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> s;
      if(s == "processes")
      {
        linestream >> processes;
        break;
      }
    }
    stream.close();
  }
  return processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory+kStatFilename);
  string s,line;
  int running_processes;
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> s;
      if(s == "procs_running")
      {
        linestream >> running_processes;
        break;
      }
    }
    stream.close();
  }
  return running_processes;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{
  string line;
  std::ifstream stream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if(stream.is_open()){
    getline(stream,line);
    stream.close();
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatusFilename);
  string s,line;
  double ram;
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> s;
      if(s == "VmSize:")
      {
        linestream >> ram;
        break;
      }
    }
    stream.close();
  }
  std::stringstream ss;
  ss << std::setprecision(3) << (0.001*ram);
  return ss.str();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string s;
  string uid;
  string line;
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> s;
      if(s == "Uid:")
      {
        linestream >> uid;
        break;
      }
    }
    stream.close();
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::ifstream stream(kPasswordPath);
  string uid = Uid(pid);
  string x;
  string id;
  string username;
  string line;
  if(stream.is_open())
  {
    while(getline(stream,line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> x >> id;
      if(id == uid)
        break;
    }
    stream.close();
  }
  return username;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) 
{
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line;
  string s;
  long start_time,uptime;
  if(stream.is_open())
  {
    getline(stream,line);
    std::istringstream linestream(line);
    for(int i=1;i<=22;i++)
    {
      linestream >> s;
    }
    start_time = std::stol(s)/sysconf(_SC_CLK_TCK);
    stream.close();
  }
  uptime = LinuxParser::UpTime() - start_time;
  return uptime;
}

