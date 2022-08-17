#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { 
    Processor processor (LinuxParser::CpuUtilization());
    cpu_ = processor;
    return cpu_; 
}
    
vector<Process>& System::Processes() { 
 
    for(size_t i= 0 ; i<LinuxParser::Pids().size(); ++i ){
        Process dummy(LinuxParser::Pids()[i],LinuxParser::User(LinuxParser::Pids()[i]) ,LinuxParser::Command(LinuxParser::Pids()[i]),LinuxParser::CpuUtilization(LinuxParser::Pids()[i]),LinuxParser::Ram(LinuxParser::Pids()[i]),LinuxParser::UpTime(LinuxParser::Pids()[i]));
        processes_.push_back(dummy);
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() {
     return LinuxParser::MemoryUtilization(); 
}

std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}
