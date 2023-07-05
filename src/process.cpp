#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Contructor of the class. It is initialized with the pid
Process::Process(int pid) : _pid(pid) { cpuUtilization = -1.0f; }

// Return this process's ID
int Process::Pid() { return _pid; }

// Return this process's CPU utilization
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float Process::CpuUtilization() {
  string aux, line;
  float utime, stime, cutime, cstime, starttime, total_time, seconds;
  int i = 0;
  std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(_pid) +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (i < 22) {
      linestream >> aux;
      if (i == 13)
        utime = stof(aux);
      else if (i == 14)
        stime = stof(aux);
      else if (i == 15)
        cutime = stof(aux);
      else if (i == 16)
        cstime = stof(aux);
      else if (i == 21)
        starttime = stof(aux);
      i++;
    }
  }

  total_time = utime + stime + cutime + cstime;
  seconds = LinuxParser::UpTime() - starttime / (float)sysconf(_SC_CLK_TCK);
  return total_time / sysconf(_SC_CLK_TCK) / seconds;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(_pid); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(_pid); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(_pid); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  bool retVal = false;

  if ((cpuUtilization >= 0.0f) && (a.cpuUtilization >= 0.0f)) {
    retVal = (cpuUtilization < a.cpuUtilization);
  }

  return retVal;
}