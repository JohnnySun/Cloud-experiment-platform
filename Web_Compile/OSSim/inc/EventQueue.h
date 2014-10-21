/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#ifndef _EVENT_QUEUE_H
#define _EVENT_QUEUE_H

#include <set>

#include "Event.h"

using namespace std;

class EventQueue {
private:
    set<Event> eventQueue;

public:
    void addEvent(Event e);

    void removeEvent(Event e);

    bool getNextEvent(Event &e);
    
    bool removeNextEvent(Event &e);
    
    void printEventQueue(long systemTime);
};
#endif /* _EVENT_QUEUE_H */
