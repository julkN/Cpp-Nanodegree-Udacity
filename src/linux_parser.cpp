#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), ::isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string memTotal, memFree;
  string line, key, num;
  float memUtln;
  vector<string> data;
  std::ifstream fileStream(kProcDirectory + kMeminfoFilename);
  if(fileStream.is_open()){
    for (int i=0; i<6; i++){ //Just the first 6 lines are important
      std::getline(fileStream, line);
      std::istringstream linestream(line);
      linestream>>key>>num;
      data.push_back(num);
    }
    memUtln =(std::stof(data[0]) - std::stof(data[1])) / std::stof (data[0]);
  }
  return memUtln;
}


long LinuxParser::UpTime() {
  string upline, line;
  std::ifstream filestream("/proc/uptime"); 
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream>>upline;
    return stol(upline);
  }
  return 0; 
}

long LinuxParser::Jiffies() { return 0; }

long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

long LinuxParser::ActiveJiffies() { return 0; }

long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() { 
  string key,line;
  vector<string> sTmpReadData;
  std::ifstream filestream("/proc/stat"); //Input file stream
  if (filestream.is_open()){
    while(std::getline(filestream, line)){ //get the string from the file
        std::istringstream linestream(line); //read the file each space
        linestream>>key;
        if(key == "cpu"){
          while(linestream){
            linestream>>key;
            sTmpReadData.push_back(key);
          }  
        }
    }    
  }
  return sTmpReadData; 
}

int LinuxParser::TotalProcesses() { 
  string value, key, line;
  std::ifstream filestream("/proc/stat");
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream>>key;
      if(key=="processes"){
        linestream>>value;
        return std::stoi(value);
      }  
    }
  }
  return 0;  //default
}

int LinuxParser::RunningProcesses() { 
  string key, line, value;
  std::ifstream filestream("/proc/stat");
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream>>key;
      if(key=="procs_running"){
        linestream>>value;
        return stoi(value);
      }
    }
  }
  return 0; //default
}

float LinuxParser::CpuUtilization(int pid){
  vector<string> data;
  string line, key;
  float utime = 0.0f;
  float stime = 0.0f;
  float cutime = 0.0f;
  float cstime= 0.0f;
  float totaltime ;
  long int starttime;
  float seconds;
  float cpu_utilization = 0.0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> key){
      data.push_back(key);
    }
  }
  if (data.size() > 20){
    utime     = std::stof(data[13]);
    stime     = std::stof(data[14]);
    cutime    = std::stof(data[15]);
    cstime    = std::stof(data[16]);
    starttime = std::stof(data[21]);
  }

  totaltime = utime + stime + cutime + cstime;
  seconds   = (float)LinuxParser::UpTime()-((starttime+0.0000001f)/(float)sysconf(_SC_CLK_TCK));
  cpu_utilization = ((totaltime+0.0000001f)/(float)sysconf(_SC_CLK_TCK))/seconds;
  return cpu_utilization;

}

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream("/proc/"+ std::to_string(pid)+"/cmdline");
  if(filestream.is_open()){
    std::getline (filestream, line);
    return line;
  }
  return string(); 
  }

string LinuxParser::Ram(int pid) { 
  string line, key, value;
  int tmp;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + "/status");
  if(filestream.is_open()){
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      linestream>>key;
      if(key == "VmSize:"){
        linestream >> value;
      }
    }
    tmp = std::stoi(value)/1024;
    return std::to_string(tmp);
  }
  return "NOT NULL"; 
}

string LinuxParser::Uid(int pid) { 
  string key, line, value;
  std::ifstream filestream("/proc/"+std::to_string(pid)+"/status");
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "Uid:"){
        linestream >>value;
        return value;
      }   
    }
  }
  return string(); 
}

string LinuxParser::User(int pid) { 
  //return "Null";
  auto uid = LinuxParser::Uid(pid);
  string key, line, value_a, value_b, x;
  std::ifstream filestream("/etc/passwd");
  if(filestream.is_open()){
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream  linestream(line);
      linestream >> key >> x >> value_a >> value_b;
      if(value_a == uid && value_b == uid){
        return key;
      }
    }
    
  }
  return string(); 
}

long LinuxParser::UpTime(int pid) { 
  string line, value;
  long int uptime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i=0; i<22 ; i++){
      linestream>>value;
    }
    uptime= std::stol(value)/sysconf(_SC_CLK_TCK);
    return uptime;
  }
  return 0; 
}
