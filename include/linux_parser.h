#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {

using std::string;

// Paths
const string kProcDirectory{"/proc/"};
const string kCmdlineFilename{"/cmdline"};
const string kCpuinfoFilename{"/cpuinfo"};
const string kStatusFilename{"/status"};
const string kStatFilename{"/stat"};
const string kUptimeFilename{"/uptime"};
const string kMeminfoFilename{"/meminfo"};
const string kVersionFilename{"/version"};
const string kOSPath{"/etc/os-release"};
const string kPasswordPath{"/etc/passwd"};

const string keyProcesses("processes");
const string keyRunningProcesses("procs_running");
const string keyMemTotalString("MemTotal:");
const string keyMemFreeString("MemFree:");
const string keyCpu("cpu");
const string keyUID("Uid:");
const string keyRam("VmRSS:");
const string keyOperatingSystem("PRETTY_NAME");

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
string OperatingSystem();
string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
string Command(int pid);
string Ram(int pid);
string Uid(int pid);
string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif