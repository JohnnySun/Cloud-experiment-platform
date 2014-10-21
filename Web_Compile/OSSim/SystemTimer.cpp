/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

#include "SystemTimer.h"

using namespace std;

SystemTimer::SystemTimer(long sysCallTime,
    long timerInterval,
    vector<Process> *processes,
    EventQueue *events) {
    this->sysCallTime = sysCallTime;
    this->timerInterval = timerInterval;
    this->processes = processes;
    this->events = events;

    snprintf(currentProcNameStr, 64, "None");;

    idleProcess = new Process("Idle", 0);

    kernelTime = 0;
    userTime = 0;
    systemTime = 0;
    idleTime = 0;
}

void SystemTimer::setKernel(Kernel *kernel) {
    osKernel = kernel;
}

void SystemTimer::processEvents() {
    while (!simulationFinished()) {
        Event nextEvent;
        bool gotNextEvent = events->getNextEvent(nextEvent);
        while (gotNextEvent != false &&
               nextEvent.getTime() <= systemTime) {
            /* print out the current pending events */
            events->printEventQueue(systemTime);

            events->removeNextEvent(nextEvent);
            processEvent(nextEvent);
            gotNextEvent = events->getNextEvent(nextEvent);
        }

        string runningProcName = 
            osKernel->chooseRunningProcess(this);
        string currentProcName(currentProcNameStr);

        if (!(equals(currentProcName, runningProcName))) {
            cout << "At system time ";
            cout << systemTime;
            cout << ", scheduler switches out ";
            cout << currentProcName << ", and chooses ";
            cout << runningProcName << " to run" << endl; 
            cout << endl;
            currentProcName = runningProcName;
            snprintf(currentProcNameStr, 64, "%s", currentProcName.c_str());
        }

        Process *runningProc = getProcess(runningProcName);

        creditProcess(runningProc);
    }

    osKernel->onAllProcessesDone(this);
}

void SystemTimer::creditProcess(Process *proc) {
    string procName = proc->getName();
    if (equals(procName, "Idle")) {
        idleTime++;
        systemTime++;
    }
    else {
        ProcessOperation pOp = proc->getOp();
        int theOp = pOp.getOpType();
        if (theOp == ProcessOperation::CPU) {
            proc->execute(1);
            userTime++;
            systemTime++;

            if (proc->getCPUBurstRemaining() == 0) {
                generateSystemCall(proc);
            }
        }
        else {
            cerr << "Process (" << procName << ")";
            cerr << "chosen to run by kernel is ";
            cerr << "not runnable! It is currently waiting ";
            cerr << "or has already exited" << endl;
            exit(-1);
        }
    }
}

void SystemTimer::generateSystemCall(Process *proc) {

    ProcessOperation pOp = proc->getOp();
    int theOp = pOp.getOpType();

    if (theOp == ProcessOperation::EXIT) {
        proc->setFinishTime(systemTime);
        osKernel->systemCall(
            Kernel::TERMINATE_PROCESS, proc->getName(), this);
        kernelTime = kernelTime + sysCallTime;
        systemTime = systemTime + sysCallTime;
    }
    else {
        cerr << "SystemTimer.generateSystemCall()";
        cerr << " should not be handling: ";
        cerr << pOp.toString() << endl;
        exit(-1);
    }
}

void SystemTimer::processEvent(Event e) {
    if (e.isDevEvt()) {
        osKernel->onInterrupt(e.getDevEvt(), this);
        if (equals(e.getDevEvt(), "TIMER")) {
            scheduleTimerInterrupt(timerInterval);
        }
        kernelTime = kernelTime + sysCallTime;
        systemTime = systemTime + sysCallTime;
    }
    else if (e.isPoPEvt()) {
        ProcessOperation pOp = e.getPoPEvt();
        int theOp = pOp.getOpType();

        if (theOp == ProcessOperation::START) {
            osKernel->systemCall(
                Kernel::START_PROCESS,
                pOp.getParent()->getName(),
                this);
            kernelTime = kernelTime + sysCallTime;
            systemTime = systemTime + sysCallTime;
        }
        else if (theOp == ProcessOperation::IO) {
            osKernel->onInterrupt(pOp.getOpDevice(), this);
            kernelTime = kernelTime + sysCallTime;
            systemTime = systemTime + sysCallTime;

            pOp.getParent()->advanceOpCounter();
        }
        else {
            cerr << "Event " << pOp.toString();
            cerr << " should not have been found";
            cerr << " in the event queue." << endl;
            exit(-1);
        }
    }
    else {
        cerr << "Unrecognized Event ";
        cerr << " has been found in the event queue." << endl;
        exit(-1);
    }
}

Process *SystemTimer::getProcess(string procName) {
    string upperCaseProcName = procName;
    to_upper(upperCaseProcName);

    if (equals(upperCaseProcName, "IDLE"))
        return idleProcess;

    bool processInList = false;
    Process *proc;
    for (int i=0; i<processes->size(); i++) {
        if (equals(procName, processes->at(i).getName())) {
            processInList = true;
            proc = &(processes->at(i));
            break;
        }
    }

    if (processInList) {
        return proc;
    } else {
        cerr << "Process (" << procName << ")";
        cerr << " chosen to run by ";
        cerr << " kernel does not exist." << endl;
        exit(-1);
    }
}

bool SystemTimer::simulationFinished() {
    bool allDone = true;
    for (int i=0; i<processes->size(); i++) {
        allDone = allDone && processes->at(i).finished();
    }

    return allDone;
}

void SystemTimer::scheduleTimerInterrupt(int delay) {
    cancelTimerInterrupt();
    if (delay > 0)
    {
        Event currentTimerEvent(
            systemTime + delay + sysCallTime,
            "TIMER");
        events->addEvent(currentTimerEvent);
    }
}

int SystemTimer::getTimerTicksRemaining() {
    if (currentTimerEvent != NULL) {
        long timerTime = currentTimerEvent->getTime();
        if (timerTime >= systemTime) {
            return (int)(timerTime - systemTime);
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}

void SystemTimer::cancelTimerInterrupt() {
    if (currentTimerEvent != NULL) {
        events->removeEvent(*currentTimerEvent);
        currentTimerEvent = NULL;
    }
}

float SystemTimer::printTimingStats() 
{
    cout << "==========================================" << endl;
    cout << "Scheduling statistics:" << endl;
    cout << "==========================================" << endl;
    cout << "System Time: " << systemTime << endl;
    cout << "Kernel Time: " << kernelTime << endl;
    cout << "  User Time: " << userTime << endl;
    cout << "  Idle Time: " << idleTime << endl;

    cout << "CPU Utilization: ";
    cout << userTime*1.0/systemTime << endl;

    cout << endl;

    int procTT;
    int procDT;
    float procWTT;
    float systemWTT = 0.0;

    cout << fixed;
    cout.precision(3);

    cout << "(Turnaround Time):(Duration):";
    cout << "(Weighted Turnaround Time)" << endl;
    for (int i=0; i<processes->size(); i++) { 
        procTT = processes->at(i).getFinishTime() -
              processes->at(i).getArrivalTime();
        procDT = processes->at(i).getDuration();
        procWTT = procTT*1.0/procDT; 
        systemWTT += procWTT;

        cout << processes->at(i).getName();
        cout << "(" << procTT << ":" << procDT << ":" << procWTT;
        cout << ")"  << endl; 
    }

    systemWTT /= processes->size();
    cout << "System Weighted Turnaround Time (your algorithm): ";
    cout << systemWTT << endl;
    cout << endl;
    
    return systemWTT;
}
