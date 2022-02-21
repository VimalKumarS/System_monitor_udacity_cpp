#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> cpu_list = LinuxParser::CpuUtilization();
    long user, nice, system, irq, softirq, steal, idle, iowait;

    user = std::stol(cpu_list[LinuxParser::CPUStates::kUser_]);
    nice = std::stol(cpu_list[LinuxParser::CPUStates::kNice_]);
    system = std::stol(cpu_list[LinuxParser::CPUStates::kSystem_]);
    irq = std::stol(cpu_list[LinuxParser::CPUStates::kIRQ_]);
    softirq = std::stol(cpu_list[LinuxParser::CPUStates::kSoftIRQ_]);
    steal = std::stol(cpu_list[LinuxParser::CPUStates::kSteal_]);

    idle = std::stol(cpu_list[LinuxParser::CPUStates::kIdle_]);
    iowait = std::stol(cpu_list[LinuxParser::CPUStates::kIOwait_]);

    long PrevIdle = previdle + previowait;
    long Idle = idle + iowait;

    long PrevNonIdle =
            prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    long NonIdle = user + nice + system + irq + softirq + steal;
    long PrevTotal = PrevIdle + PrevNonIdle;
    long Total = Idle + NonIdle;
    long totald = Total - PrevTotal;
    long idled = Idle - PrevIdle;
    long CPU_Percentage = (totald - idled) / totald;

    prevuser = user;
    prevnice = nice;
    prevsystem = system;
    previrq = irq;
    prevsoftirq = softirq;
    prevsteal = steal;
    previdle = idle;
    previowait = iowait;

    return CPU_Percentage;

}



//user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
//cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
//
//PrevIdle = previdle + previowait
//Idle = idle + iowait
//
//PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
//NonIdle = user + nice + system + irq + softirq + steal
//
//PrevTotal = PrevIdle + PrevNonIdle
//Total = Idle + NonIdle
//
//# differentiate: actual value minus the previous one
//totald = Total - PrevTotal
//idled = Idle - PrevIdle
//
//CPU_Percentage = (totald - idled)/totald