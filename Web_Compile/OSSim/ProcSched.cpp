/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#include <string>
#include <iostream>

#include "ProcSched.h"

 
using namespace std;
int i=1;
void ProcSched::systemCall(int callID, string param, SystemTimer *timer) {
    // For logging output, DO NOT EDIT this line
    printSystemCall(callID, param, timer->getSystemTime());

    if (callID == Kernel::START_PROCESS) {
        // Create a PCB for the new process
        // and put it on the back of the ready queue.
        readyQueue.push_back(ProcessControlBlock(param));
    }
    else if (callID == Kernel::TERMINATE_PROCESS) {
        // Assume the first process in the ready queue is the
        // one that is running and remove it.
      i=0;
        readyQueue.erase(readyQueue.begin() + runningProcIdxInQueue);
    }
    else if (callID == Kernel::IO_REQUEST) {
        // This would execute if the executing process
        // were to make an I/O request.
    }
    else if (callID == Kernel::MAKE_DEVICE) {
        // This is executed at system startup once for each
        // device listed in the devices file.
    }
}

void ProcSched::onInterrupt(string deviceID, SystemTimer *timer) {
        if (equals(deviceID, "TIMER")) {
        if(i) {
        string s=readyQueue[0].getName(); 
        readyQueue.erase(readyQueue.begin() + runningProcIdxInQueue);
        readyQueue.push_back(ProcessControlBlock(s));}
            // This would execute if a TIMER intrrupt occurs.
        else 
		{	i=1; }

        }
        else {
            // This would run if the interrupt came from a
            // device. Hopefully there was a MAKE_DEVICE call for
            // the device earlier - but keep in mind that there is
            // a chance the device doesn't exist. In that case
            // simulator should report error and exit
        }
    }
 

string ProcSched::chooseRunningProcess(SystemTimer *timer) {
    // Simple FCFS scheduling assumes that the process at
    // the front of the ready queue is the process in the
    // running state.
      
      if(readyQueue.size() > 0) {
       runningProcIdxInQueue = 0;
       string proc = readyQueue[runningProcIdxInQueue].getName(); 
       return proc; 
       } 
     else {
        return "Idle";
    }
    
}

void ProcSched::onAllProcessesDone(SystemTimer *timer) {
}
