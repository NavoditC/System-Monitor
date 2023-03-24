#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "processor.h"
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
  public:
    Process(int pid);
    int Pid();                               
    std::string User();                      
    std::string Command();                   
    float CpuUtilization();                 
    std::string Ram();                       
    long UpTime();                       
    bool operator<(Process const& a) const;  

  // Declare necessary private members
  private:
    int pid;
    float cpu_utilization;
};

#endif