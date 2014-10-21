/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#ifndef _EVENT_H
#define _EVENT_H

#include <string>

#include "ProcessOperation.h"

using namespace std;

enum {
    EVT_DEV = 0,
    EVT_POP,
    EVT_UNKNOWN
};
 
class Event {
private:
    string devEvt;
    ProcessOperation pOpEvt;
    long eventID;
    static long nextID;

public:
    long arrivalTime;
    int evtType;

    Event();
    Event(long arrivalTime, string devEvt);
    Event(long arrivalTime, ProcessOperation pOpEvt);
    
    long getTime() { return arrivalTime; }
    bool isDevEvt() { return evtType == EVT_DEV; }
    bool isPoPEvt() { return evtType == EVT_POP; }
    string getDevEvt() { return devEvt; }
    ProcessOperation getPoPEvt() { return pOpEvt; }
    bool operator<(const Event& e) const;
    
    string toString();
};

#endif /* _EVENT_H */
