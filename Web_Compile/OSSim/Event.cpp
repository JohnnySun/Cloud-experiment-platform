/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#include <string>
#include <iostream>
#include <sstream>

#include "Event.h"

using namespace std;

long Event::nextID = 0;

Event::Event() {
    arrivalTime = 0;
    evtType = EVT_UNKNOWN;
    eventID = -1;
}

Event::Event(long arrivalTime, string devEvt) {
    this->arrivalTime = arrivalTime;
    this->evtType = EVT_DEV;
    this->devEvt = devEvt;
    eventID = nextID;
    nextID++;
}

Event::Event(long arrivalTime, ProcessOperation pOpEvt) {
    this->arrivalTime = arrivalTime;
    this->evtType = EVT_POP;
    this->pOpEvt = pOpEvt;
    eventID = nextID;
    nextID++;
}

bool Event::operator<(const Event& e) const
{
    if (arrivalTime < e.arrivalTime) {
        return true;
    }
    else if (arrivalTime > e.arrivalTime) {
        return false;
    }
    else if (eventID < e.eventID) {
        return true;
    }
    else if (eventID > e.eventID) {
        return false;
    }
    else {
        // arbitarily make comparee greater
        // with nextID this will not happen, because
        // nextID ensured uniqueness.
        // this branch is for conservation.
        return false;
    }
}

string Event::toString()
{
    stringstream ss;

    if (isDevEvt()) {
        ss << getDevEvt() << " interrupt at time ";
        ss << getTime() << endl;
    } else if (isPoPEvt()) {
        ss << getPoPEvt().toString() << " at time ";
        ss << getTime() << endl;
    } else {
        ss << "unknown event" << endl;
    }

    return ss.str();
}
