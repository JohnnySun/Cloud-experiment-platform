/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#ifndef _PROCESS_H
#define _PROCESS_H

#include <string>

#include "ProcessOperation.h"

using namespace std;

static const int DEFAULT_PRIORITY = 10;

class Process {
private:
    string processName;
    int arrivalTime;
    int finishTime;
    int duration;
    int priority;
    vector<ProcessOperation> processOps;
    int opCounter;
    int cpuBurstRemaining;
 
public:
    Process(string name, int arrivalTime, int priority = DEFAULT_PRIORITY); 

    bool equals(Process p);

    void addOp(ProcessOperation pOp);

    ProcessOperation getOp();

    ProcessOperation getOp(int index);

    string getName();

    int getArrivalTime();

    int getFinishTime();

    void setFinishTime(int finishTime);

    int getDuration();

    int getOpCounter();

    void advanceOpCounter();

    void execute(int ticks);

    int getPriority();

    void setPriority(int priority);

    bool finished();
    
    int getCPUBurstRemaining();

    string toString();
};

#endif /* _PROCESS_H */
