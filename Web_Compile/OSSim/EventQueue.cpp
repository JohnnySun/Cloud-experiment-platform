/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#include <string>
#include <iostream>

#include "EventQueue.h"

using namespace std;

void EventQueue::addEvent(Event e) {
    eventQueue.insert(e);
}

void EventQueue::removeEvent(Event e) {
    set<Event>::iterator set_iter = eventQueue.begin();
    while(set_iter != eventQueue.end()) {
        if (set_iter->arrivalTime == e.arrivalTime && 
            set_iter->evtType == e.evtType)
        {
            eventQueue.erase(set_iter++); 
        } else {
            ++set_iter;
        }
    }
}

bool EventQueue::getNextEvent(Event &e) {
    if (eventQueue.empty())
        return false;

    e = *eventQueue.begin();

    return true;
}

bool EventQueue::removeNextEvent(Event &e) {
    if (eventQueue.empty())
        return false;

    getNextEvent(e);
    eventQueue.erase(eventQueue.begin());

    return true;
}
   
void EventQueue::printEventQueue(long systemTime) {
    Event e;
    set<Event>::iterator set_iter = eventQueue.begin();
    
    cout << "==========================================" << endl;
    cout << "Pending events at system time: ";
    cout << systemTime << endl;
    cout << "==========================================" << endl;

    for (; set_iter != eventQueue.end(); set_iter++) {
        e = *set_iter;
        cout << e.toString();
    }
 
    cout << endl;
}
