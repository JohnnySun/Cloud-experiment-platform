/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

#include "Process.h"

Process::Process(string name, int arrivalTime, int priority) {
    processName = name;
    this->arrivalTime = arrivalTime;
    this->priority = priority;
    finishTime = arrivalTime;
    duration = 0;
    opCounter = 0;
    cpuBurstRemaining = 0;
}

bool Process::equals(Process p) {
    return boost::equals(processName, p.processName);
}

void Process::addOp(ProcessOperation pOp) { 
    processOps.push_back(pOp);
    duration += pOp.getOpTime();
}

ProcessOperation Process::getOp() {
    return getOp(opCounter);
}

ProcessOperation Process::getOp(int index) {
    return processOps[index];
}

string Process::getName() {
    return processName;
}

int Process::getArrivalTime() {
    return arrivalTime;
}

int Process::getFinishTime() {
    return finishTime;
}

void Process::setFinishTime(int finishTime) {
    this->finishTime = finishTime;
} 
 
int Process::getDuration() {
    return duration;
}

int Process::getOpCounter() {
    return opCounter;
}

void Process::advanceOpCounter() {
    opCounter++;

    ProcessOperation pOp = getOp();
    int theOp = pOp.getOpType();

    if (theOp == ProcessOperation::CPU) {
        cpuBurstRemaining = pOp.getOpTime();
    }
}

void Process::execute(int ticks) {
    ProcessOperation pOp = getOp();
    int theOp = pOp.getOpType();

    if (theOp == ProcessOperation::EXIT) {
        cerr << "Process " <<  processName;  
        cerr << " has already terminated! ";
        cerr << "it can not be executed!" << endl;
        exit(-1);
    }
    else {
        cpuBurstRemaining = cpuBurstRemaining - ticks;
        if (cpuBurstRemaining <= 0) {
            cpuBurstRemaining = 0;
            advanceOpCounter();
        }
    }
}

bool Process::finished() {
    ProcessOperation pOp = getOp();
    int theOp = pOp.getOpType();
    return (theOp == ProcessOperation::EXIT);
}

int Process::getCPUBurstRemaining() {
    return cpuBurstRemaining;
}

int Process::getPriority() {
    return priority;
}

void Process::setPriority(int priority) {
    this->priority = priority;
}

string Process::toString() {
    string str;
    stringstream ss;

    ss << arrivalTime;
    
    str = getName() + " (arrives at time " + ss.str() +  ") :\n";
    for (int i=0; i<processOps.size(); i++) { 
        str = str + "\t" + processOps[i].toString() + "\n";
    }

    return str;
}
