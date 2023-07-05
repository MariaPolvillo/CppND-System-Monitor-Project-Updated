#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  NCursesDisplay::Display(system);
  return 0;
}

/* Tests to debug the code
#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

int main() {
  std::vector<std::string> v = LinuxParser::CpuUtilization();
  for (auto &elem : v) std::cout << elem << " ";
  std::cout << "" << std::endl;

  Processor processor;
  float cpuutilization = processor.Utilization();
  std::cout << "CPU Utilization = " << cpuutilization << std::endl;

  std::string command = LinuxParser::Command(843);
  std::cout << "Command = " << command << std::endl;

  std::string ram = LinuxParser::Ram(843);
  std::cout << "Ram = " << ram << std::endl;

  std::string uid = LinuxParser::Uid(843);
  std::cout << "Uid = " << ram << std::endl;

  std::string user = LinuxParser::User(843);
  std::cout << "User = " << user << std::endl;

  long uptime = LinuxParser::UpTime(843);
  std::cout << "Uptime = " << uptime << std::endl;

  return 0;
}
*/