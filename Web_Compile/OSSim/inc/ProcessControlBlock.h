/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#ifndef _PROCESS_CONTROL_BLOCK_H
#define _PROCESS_CONTROL_BLOCK_H

#include <string>

using namespace std;

class ProcessControlBlock {

private:
    string name;

public:
    ProcessControlBlock(string name) {
        this->name = name;
    }

    string getName() {
        return name;
    }
};

#endif /* _PROCESS_CONTROL_BLOCK_H */
