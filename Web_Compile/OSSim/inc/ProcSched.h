/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#ifndef _PROC_SCHED_H
#define _PROC_SCHED_H

#include <vector>

#include "Kernel.h"
#include "ProcessControlBlock.h"
#include "SystemTimer.h"

using namespace std;

class ProcSched: public Kernel {
private:
    vector<ProcessControlBlock> readyQueue;
    int runningProcIdxInQueue;

public:
    virtual void systemCall(int callID, string param, SystemTimer *timer);

    virtual void onInterrupt(string deviceID, SystemTimer *timer);

    virtual string chooseRunningProcess(SystemTimer *timer);

    virtual void onAllProcessesDone(SystemTimer *timer);
};

#endif /* _PROC_SCHED_H */
