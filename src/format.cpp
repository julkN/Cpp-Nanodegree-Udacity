#include <string>
#include <sstream>

#include "format.h"

using std::string;

string Format::Num2String(const long numb){
    std::stringstream ss;
    if(numb<10)
        ss << "0";

    ss << numb;
    return ss.str();    
}

string Format::ElapsedTime(long seconds) { 
    long lHH(seconds/3600);
    long lH_re(seconds%3600);
    long lMM(lH_re/60);
    long lSS(lH_re%60);
    string sHH (Num2String(lHH));
    string sMM (Num2String(lMM));
    string sSS (Num2String(lSS));
    string time(sHH+':'+sMM+':'+sSS);
    return time; 
    }