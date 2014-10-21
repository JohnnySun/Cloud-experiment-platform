/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#ifndef _PROCESS_OPERATION_H
#define _PROCESS_OPERATION_H

#include <string>

using namespace std;

class Process;

class ProcessOperation {
public:
    enum ProcOperationType {
        CPU = 1,
        IO,
        WAIT,
        NOTIFY,
        NOTIFY_ALL,
        START,
        EXIT
    };

private:
    Process *parent;
    int opType;
    string opDevice;
    int opTime;

public:
    ProcessOperation() {}

    ProcessOperation(string line, Process *parent);

    int getOpType() {
        return opType;
    }

    string getOpDevice() {
        return opDevice;
    }

    int getOpTime() {
        return opTime;
    }

    Process *getParent() { return parent; }

    string toString();
};

#endif /* _PROCESS_OPERATION_H */
