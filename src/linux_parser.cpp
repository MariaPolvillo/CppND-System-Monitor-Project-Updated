#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// *********** SYSTEM *************** //
// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line = "";
  string key = "";
  int value = 0;
  float mem_total = 0.0f, mem_free = 0.0f, retVal = 0.0f;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyMemTotalString)
          mem_total = static_cast<float>(value * 9.5367e-7);
        else if (key == keyMemFreeString) {
          mem_free = static_cast<float>(value * 9.5367e-7);
          break;
        }
      }
    }
  }

  if (mem_total > 0.0f) retVal = (mem_total - mem_free) / mem_total;

  return retVal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime = 0.0l;
  string line = "";
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return uptime;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids = {};
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

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line = "";
  string key = "";
  int value = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyProcesses) return value;
      }
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line = "";
  string key = "";
  int value = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyRunningProcesses) return value;
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line = "";
  string key = "", value = "";
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyOperatingSystem) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os = "", version = "", kernel = "";
  string line = "";
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// *********** CPU *************** //
// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line = "", cpu_id = "";
  int user = 0, nice = 0, system = 0, idle = 0, iowait = 0, irq = 0,
      softirq = 0, steal = 0, guest = 0, guest_nice = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu_id >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
  }

  vector<string> cpu_elem = {to_string(user),    to_string(nice),
                             to_string(system),  to_string(idle),
                             to_string(iowait),  to_string(irq),
                             to_string(softirq), to_string(steal),
                             to_string(guest),   to_string(guest_nice)};

  return cpu_elem;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() / sysconf(_SC_CLK_TCK); }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long user_jiffies = 0.0l, kernel_jiffies = 0.0l;
  string aux = "", line = "";

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> aux >> aux >> aux >> aux >> aux >> aux >> aux >> aux >> aux >>
        aux >> aux >> aux >> aux >> user_jiffies >> kernel_jiffies;
  }

  return user_jiffies + kernel_jiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long user = 0.0l, nice = 0.0l, system = 0.0l, irq = 0.0l, softirq = 0.0l,
       steal = 0.0l, aux = 0.0l;
  string cpu_name = "", line = "";

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu_name >> user >> nice >> system >> aux >> aux >> irq >>
        softirq >> steal;
  }

  return user + nice + system + irq + softirq + steal;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  int idle = 0, iowait = 0, aux = 0;
  string cpu_name = "", line = "";

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu_name >> aux >> aux >> aux >> idle >> iowait;
  }

  return idle + iowait;
}

// *********** PROCESSES *************** //
// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd = "", line = "";
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmd;
  }

  cmd  = cmd.substr(0, 50) + "..."; 

  return cmd;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string vm_size = "", line = "";
  int mem = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> vm_size >> mem;
      // VmRSS intead VmSize (for physical RAM usage)
      // https://man7.org/linux/man-pages/man5/proc.5.html
      if (vm_size == keyRam) {
        break;
      }
    }
  }

  return to_string(mem / 1000);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string uid_pid = "", uid = "", line = "";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> uid_pid >> uid) {
        if (uid_pid == keyUID) {
          break;
        }
      }
    }
  }

  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string userid = Uid(pid);
  std::string user = "", uid = "", x = "", line;

  std::ifstream stream(kPasswordPath);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid) {
        if (userid == uid) return user;
      }
    }
  }

  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  int aux = 0, uptimepid = 0;
  string aux_s = "", line = "";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> aux >> aux_s >> aux_s >> aux >> aux >> aux >> aux >> aux >>
        aux >> aux >> aux >> aux >> aux >> aux >> aux >> aux >> aux >> aux >>
        aux >> aux >> aux >> uptimepid;
  }

  long uptime = LinuxParser::UpTime();

  return static_cast<long>((uptime - (uptimepid / sysconf(_SC_CLK_TCK))));
}
