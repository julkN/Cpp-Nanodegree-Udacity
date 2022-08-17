#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include <vector>

using std::string;
using std::vector;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor(){}
  Processor(vector<string> processor){
    user_ = std::stoi(processor[0]);
    nice_ = std::stoi(processor[1]);
    system_ = std::stoi(processor[2]);
    idle_ = std::stoi(processor[3]);
    iowat_ = std::stoi(processor[4]);
    irq_ = std::stoi(processor[5]);
    softirq_ = std::stoi(processor[6]);
    steal_ = std::stoi(processor[7]);
    guest_ = std::stoi(processor[8]);
    guest_nice_ = std::stoi(processor[9]);
  }
  // TODO: Declare any necessary private members
 private:
   int user_;
   int nice_;
   int system_;
   int idle_;
   int iowat_;
   int irq_;
   int softirq_;
   int steal_;
   int guest_;
   int guest_nice_;

};

#endif