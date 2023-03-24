#include <iostream>
#include <string>
#include "format.h"
#include <stdexcept> 
using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::number_to_string(long n)
{
  if(n<=9)
    return "0" + to_string(n);
  else
    return to_string(n);
}
string Format::ElapsedTime(long seconds) 
{
  long hours,mts,sec,time_remaining;
  string time_str;
  hours = seconds/3600;
  time_remaining = seconds%3600;
  mts = time_remaining/60;
  sec = time_remaining%60;
  time_str = number_to_string(hours) + ":" + number_to_string(mts) + ":" + number_to_string(sec);
  return time_str;
}