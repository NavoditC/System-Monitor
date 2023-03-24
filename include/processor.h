#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization(); 

  // Declare necessary private members
 private:
  long prev_idle = 0;
  long prev_nonidle = 0;
  long prev_total = 0;
};

#endif