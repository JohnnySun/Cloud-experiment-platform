/* 
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#ifndef _SIM_FRAMEWORK_H
#define _SIM_FRAMEWORK_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "SystemTimer.h"
#include "Event.h"
#include "EventQueue.h"
#include "Process.h"
#include "ProcessOperation.h"
#include "Kernel.h"
#include "ProcSched.h"

using namespace std;
using namespace boost;

static void prepareScoreFile(
    ofstream &scoreFile,
    string scoreFileName);

static void scheduleProcessArrivals(
        vector<Process> &processes,
        EventQueue *events);

static void createDevicesAndQueues(
    vector<string> ioDevices,
    vector<string> queueDevices,
    Kernel *osKernel,
    SystemTimer *timer);

static void printDevicesAndQueues(
    vector<string> ioDevices,
    vector<string> queueDevices);

static void printProcesses(
    vector<Process> processes);
    
static void readDeviceList(
    string devicesFileName,
    vector<string> &ioDevices,
    vector<string> &queueDevices);

static void readProcessList(
    string processesFileName,
    string &baselineAlgorithm,
    float &expectedSystemWTT,
    int &sysCallTime,
    int &timerInterval,
    vector<string> &processList);

static void readProcessData(
    vector<string> processList,
    vector<Process> &processes,
    vector<string> ioDevices,
    vector<string> queueDevices,
    Kernel *osKernel);

static Process *readProcess(
    string procFileName,
    vector<string> ioDeviceList,
    vector<string> queueDeviceList,
    Kernel *osKernel);

static bool inList(
    string item,
    vector<string> list);

#endif /* _SIM_FRAMEWORK_H */
