#include "processor.h"

float Processor::Utilization() { 
    float fOper;
    int idle = idle_ + iowat_;
    int nonIdle = user_+nice_ + iowat_ + irq_ + softirq_ + steal_;
    int total = idle + nonIdle;
    int totald = total;
    int idled = idle;
    fOper = static_cast<float>(totald-idled) / static_cast<float>(totald); 
    return fOper;
}