#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <sstream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;


// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "PRETTY_NAME") {
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
    string os, version, kernel;
    string line;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
    vector<int> pids;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    float MemTotal, MemFree, value, finalValue;
    string key, line;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "MemTotal:") {
                    MemTotal = value;
                } else if (key == "MemFree:") {
                    MemFree = value;
                }
            }
        }
    }
    finalValue = (MemTotal - MemFree) / float(MemTotal);
    return finalValue;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    string line;
    string uptime, idletime;
    long luptime;
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> uptime >> idletime;
        luptime = std::stol(uptime);
    }

    return luptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    vector<string> stat;
    string value, line;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        while (linestream >> value) {
            stat.push_back(value);
        }
    }
    return std::stol(stat[13]) + std::stol(stat[14]) + std::stol(stat[15]) +
           std::stol(stat[16]);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    std::vector<string> cpu = CpuUtilization();
    return std::stol(cpu[kUser_]) + std::stol(cpu[kNice_]) +
           std::stol(cpu[kSystem_]) + std::stol(cpu[kIRQ_]) +
           std::stol(cpu[kSoftIRQ_]) + std::stol(cpu[kSteal_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    std::vector<string> cpu = CpuUtilization();
    return std::stol(cpu[kIdle_]) + std::stol(cpu[kIOwait_]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    string line;
    string key;
    vector<string> value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        while (linestream >> key) {
            if (key != "cpu") {
                value.push_back(key);
            }
        }
    }
    return value;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    int val;
    string key, value, line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "processes") {
                    val = std::stoi(value);
                    return val;
                }
            }
        }
    }
    return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    int val;
    string key, value, line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "procs_running") {
                    val = std::stoi(value);
                    return val;
                }
            }
        }
    }
    return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    string line;
    string value;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                             kCmdlineFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while (linestream >> value) {
                return value;
            }
        }
    }
    return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    string key, value, kb, line;
    long vmsize;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value >> kb) {
                if (key == "VmSize:") {
                    vmsize = std::stoi(value);
                    vmsize = vmsize / 1000;
                    return std::to_string(vmsize);
                    //           return (value + " " + kb);
                }
            }
        }
    }
    return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string line;
    string key;
    string value;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                             kStatusFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "Uid:") {
                    return value;
                }
            }
        }
    }
    return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    string line;
    string key, xstr;
    string value;
    vector<string> temp;
    string uid = LinuxParser::Uid(pid);
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            if (linestream >> value >> xstr >> key) {
                if (key == uid) {
                    return value;
                }
            }
        }
    }

    return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    string line;
    string key;
    long value;
    vector<string> temp;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                             kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key) {
                temp.push_back(key);
            }
        }
    }
    //    value = std::stol(temp[13]) + std::stol(temp[14]) + std::stol(temp[15])
    //    + std::stol(temp[16]);
    value = (std::stol(temp[21]) / sysconf(_SC_CLK_TCK));  // start time
    value = LinuxParser::UpTime() - value;
    return value;
}



//#14 utime - CPU time spent in user code, measured in clock ticks
//#15 stime - CPU time spent in kernel code, measured in clock ticks
//#16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
//#17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
//#22 starttime - Time when the process started, measured in clock ticks
//total_time = utime + stime
//total_time = total_time + cutime + cstime
//uptime - (starttime / Hertz)