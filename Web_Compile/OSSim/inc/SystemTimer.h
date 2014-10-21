/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#ifndef _SYSTEM_TIMER_H
#define _SYSTEM_TIMER_H

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "ProcessOperation.h"
#include "Process.h"
#include "Event.h"
#include "EventQueue.h"
#include "Kernel.h"

using namespace std;

class SystemTimer {
private:
    long sysCallTime;
    long timerInterval;
    Kernel *osKernel;
    vector<Process> *processes;
    map<string, long> deviceTimes;
    EventQueue *events;
    long kernelTime;
    long systemTime;
    long idleTime;
    long userTime;
    Event *currentTimerEvent;
    Process *idleProcess;
    char currentProcNameStr[64];

public:
    SystemTimer(long sysCallTime,
        long timerInterval,
        vector<Process> *processes,
        EventQueue *events);

    void setKernel(Kernel *kernel);

    void processEvents();

    Process *getProcess(string procName);

private:
    void creditProcess(Process *proc);

    void generateSystemCall(Process *proc);

    void processEvent(Event e);

    bool simulationFinished();

public:
    void scheduleTimerInterrupt(int delay);

    int getTimerTicksRemaining();

    void cancelTimerInterrupt();

    long getKernelTime() { return kernelTime; }

    long getUserTime() { return userTime; }

    long getIdleTime() { return idleTime; }

    long getSystemTime() { return systemTime; }

    long getSysCallTime() { return sysCallTime; }
   
    long getTimerInterval() { return timerInterval; }
  
    float printTimingStats();
};

#endif /* _SYSTEM_TIMER_H */
