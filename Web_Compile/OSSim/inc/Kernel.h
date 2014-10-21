/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#ifndef _KERNEL_H
#define _KERNEL_H

#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

class SystemTimer;

class Kernel {
public:
    enum SysCallType {
        MAKE_DEVICE = 1,
        START_PROCESS,
        IO_REQUEST,
        TERMINATE_PROCESS
    };

    virtual void systemCall(int callID, string param, SystemTimer *timer) = 0;

    virtual void onInterrupt(string deviceID, SystemTimer *timer) = 0;

    virtual string chooseRunningProcess(SystemTimer *timer) = 0;

    virtual void onAllProcessesDone(SystemTimer *timer) = 0;

    void printSystemCall(int callID, string param, int systemTime)
    {  
        cout << "System call takes place at system time ";
        cout << systemTime << ": ";

        switch (callID) {
        case MAKE_DEVICE:
            cout << "MAKE_DEVICE - " << param << endl;
            break;
        case START_PROCESS:
            cout << "START_PROCESS - " << param << endl;
            break;
        case IO_REQUEST:
            cout << "IO_REQUEST - " << param << endl;
            break;
        case TERMINATE_PROCESS:
            cout << "TERMINATE_PROCESS - " << param << endl;
            break;
        default: 
            cout << "Unknown" << endl;
        } 
        
        cout << endl;
    }
};

#endif /* _KERNEL_H */
