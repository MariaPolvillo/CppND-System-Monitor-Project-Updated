#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

#include "linux_parser.h"

class Processor {
 public:
  Processor();
  float Utilization();

 private:
  std::vector<std::string> cpu_;
  std::vector<int> prevcpu_;
};

#endif