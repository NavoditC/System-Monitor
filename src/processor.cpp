#include "processor.h"
#include <vector>
#include <string>

using std::vector;
using std::getline;
using std::string;
// Return the aggregate CPU utilization
float Processor::Utilization() { 
  std::ifstream stream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
  string line;
  string s;
  long word;
  vector<long> cpu_utilization;
  if(stream.is_open())
  {
    getline(stream,line);
    std::istringstream linestream(line);
    linestream >> s;
    if(s == "cpu"){
      while(linestream >> word)
        cpu_utilization.push_back(word);
    }
  }
  long idle = cpu_utilization[3] + cpu_utilization[4];
  long non_idle = cpu_utilization[0] + cpu_utilization[1] + cpu_utilization[2] + cpu_utilization[5] + cpu_utilization[6] + cpu_utilization[7];
  long total = idle + non_idle;
  long totald = total - prev_total;
  long idled = idle - prev_idle;
  prev_idle = idle;
  prev_nonidle = non_idle;
  prev_total = total;
  return (static_cast<float>(totald-idled))/static_cast<float>(totald);
}