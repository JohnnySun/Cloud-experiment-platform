/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "ProcessOperation.h"
#include "Process.h"

using namespace std;
using namespace boost;

ProcessOperation::ProcessOperation(string line, Process *parent) {
    trim(line);
    to_upper(line);

    this->parent = parent;
    opDevice = "";
    opTime = 0;

    if (starts_with(line, "START")) {
        opType = START;
    }
    else if (starts_with(line, "EXIT")) {
        opType = EXIT;
    }
    else if (starts_with(line, "CPU")) {
        opType = CPU;
        line = line.substr(3);
        trim(line);
        opTime = atoi(line.c_str());
    }
    else if (starts_with(line, "WAIT")) {
        opType = WAIT;
        line = line.substr(4);
        trim(line);
        opDevice = line;
    }
    else if (starts_with(line, "NOTIFY")) {
        opType = NOTIFY;
        line = line.substr(6);
        trim(line);
        opDevice = line;
    }
    else if (starts_with(line, "NOTIFY_ALL")) {
        opType = NOTIFY_ALL;
        line = line.substr(10);
        trim(line);
        opDevice = line;
    }
    else if (starts_with(line, "IO")) {
        opType = IO;
        line = line.substr(2);
        trim(line);
        vector<string> tokens;
        tokens = split(tokens, line, is_any_of(","), token_compress_on);
        opDevice = tokens[0];
        opTime = atoi(tokens[1].c_str());
    }
    else {
        cerr << "Unrecognized operation in: " << line << endl; 
        exit(-1);
    }
}

string ProcessOperation::toString() {
    stringstream ss;
    ss << opTime; 

    switch(opType) {
       case CPU: return parent->getName() + ":CPU " + ss.str();
       case IO: return parent->getName() + ":IO " + opDevice + " " + ss.str();
       case WAIT: return parent->getName() + ":WAIT " + opDevice;
       case NOTIFY: return parent->getName() + ":NOTIFY " + opDevice;
       case NOTIFY_ALL: return parent->getName() + ":NOTIFY_ALL " + opDevice;
       case START: return parent->getName() + ":START";
       case EXIT: return parent->getName() + ":EXIT";
       default: return "oops.";
    }
}
