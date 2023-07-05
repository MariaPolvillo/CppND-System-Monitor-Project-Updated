#include "processor.h"

Processor::Processor() { prevcpu_ = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; }

// Return the aggregate CPU utilization
// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux/23376195#23376195
// Process CPU Utilization information
/* PrevIdle = previdle + previowait;
  Idle = idle + iowait;
  PrevNonIdle =
      prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  NonIdle = user + nice + system + irq + softirq + steal;
  PrevTotal = PrevIdle + PrevNonIdle;
  Total = Idle + NonIdle;
  // differentiate: actual value minus the previous one */

float Processor::Utilization() {
  cpu_ = LinuxParser::CpuUtilization();
  std::vector<int> cpu_int;

  for (auto &elem : cpu_) cpu_int.emplace_back(std::atoi(elem.c_str()));

  int PrevIdle = prevcpu_[3] + prevcpu_[4];
  int Idle = cpu_int[3] + cpu_int[4];

  int PrevNonIdle = prevcpu_[0] + prevcpu_[1] + prevcpu_[2] + prevcpu_[5] +
                    prevcpu_[6] + prevcpu_[7];
  int NonIdle = cpu_int[0] + cpu_int[1] + cpu_int[2] + cpu_int[5] + cpu_int[6] +
                cpu_int[7];

  int PrevTotal = PrevIdle + PrevNonIdle;
  int Total = Idle + NonIdle;

  int totald = Total - PrevTotal;
  int idled = Idle - PrevIdle;

  // Save CPU information of this clock tick
  prevcpu_ = cpu_int;

  float retVal = 0.0f;
  if (totald != 0)
    retVal = static_cast<float>(totald - idled) / static_cast<float>(totald);

  return retVal;
}