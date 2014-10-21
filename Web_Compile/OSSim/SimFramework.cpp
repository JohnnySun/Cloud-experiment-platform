/*
 * OS Simulator written by Xun Luo (xun.luo@ieee.org)
 * Inspired by the implementation of Grant William Braught
 */

#include <math.h>
#include "SimFramework.h"

#define MAX_DEVICES 100
#define MAX_QUEUES 100
#define MAX_PROCESSES 100
#define MAX_PROCESS_OPERATION 20

int main(int argc, char **argv)
{
    if (argc == 4) {
        string processesFileName = string(argv[1]);
        string devicesFileName = string(argv[2]);
        string scoreFileName = string(argv[3]) + ".score";
        ofstream scoreFile;

        vector<string> ioDevices;
        vector<string> queueDevices;
        string baselineAlgorithm;
        float expectedSystemWTT;
        int sysCallTime;
        int timerInterval;
        vector<string> processList;
        vector<Process> processes;
        int score;

        ioDevices.reserve(MAX_DEVICES);
        queueDevices.reserve(MAX_QUEUES);
        processes.reserve(MAX_PROCESSES);

        prepareScoreFile(scoreFile, scoreFileName);

        EventQueue *events = new EventQueue();
        Kernel *osKernel = (Kernel *) new ProcSched();
            
        readDeviceList(devicesFileName, ioDevices, queueDevices);
        readProcessList(
            processesFileName,
            baselineAlgorithm,
            expectedSystemWTT,
            sysCallTime,
            timerInterval,
            processList);
        readProcessData(processList, processes, ioDevices, queueDevices,
            osKernel);

        SystemTimer *timer = new SystemTimer(sysCallTime,
                                             timerInterval,
                                             &processes,
                                             events);
        timer->setKernel(osKernel);

        createDevicesAndQueues(ioDevices, queueDevices, osKernel, timer);

        printDevicesAndQueues(ioDevices, queueDevices);
        printProcesses(processes);

        scheduleProcessArrivals(processes, events);

        timer->scheduleTimerInterrupt(timerInterval);
        timer->processEvents();

        float actualSystemWTT = timer->printTimingStats();

        if (actualSystemWTT == 0) {
            cerr << "System weighted turnaround time should not be zero! ";
            cerr << "Cannot calculate score." << endl;
            exit(-1);
        }

        score = ceilf((expectedSystemWTT/actualSystemWTT)*100);
        cout << "Expected System Weighted Turnaround Time: *** ";
        cout << expectedSystemWTT;
        cout << " (" << baselineAlgorithm <<  ") ***" << endl;
        cout << "Your score is: " << score << endl;

        scoreFile << score << endl;
        scoreFile.close();
    } else {
        cerr << "Usage: OSSim <processes> <devices> <scorefile>\n" 
                "\t <processes> - Processes data file.\n"
                "\t <devices> - Devices data file.\n"
                "\t <scorefile> - Score file\n";
        return -1;
    }

    return 0;
}

static void prepareScoreFile(
    ofstream &scoreFile,
    string scoreFileName) {
    scoreFile.open(scoreFileName.c_str(), ios::out);
    if (!scoreFile.good()) {
        cerr << "Error opening score file ";
        cerr << scoreFileName.c_str() << endl;
        exit(-1);
    }
}

static void scheduleProcessArrivals(
        vector<Process> &processes,
        EventQueue *events) {
    for (int i=0; i<processes.size(); i++) {
        Process *p = &processes[i];
        Event e(p->getArrivalTime(), p->getOp());
        p->advanceOpCounter();
        events->addEvent(e);
    }
}

static void createDevicesAndQueues(
    vector<string> ioDevices,
    vector<string> queueDevices,
    Kernel *osKernel,
    SystemTimer *timer) {
    for (int i=0; i<ioDevices.size(); i++) {
        osKernel->systemCall(
            Kernel::MAKE_DEVICE,
            ioDevices[i],
            timer);
    }
}

static void printDevicesAndQueues(
    vector<string> ioDevices,
    vector<string> queueDevices) {
    cout << "==========================================" << endl; 
    cout << "Devices and Queues in the system:" << endl;
    cout << "==========================================" << endl; 
    for (int i=0; i<ioDevices.size(); i++) {
        cout << "Device " << i << ": " << ioDevices[i] << endl;
    }
    cout << endl;
}

static void printProcesses(
    vector<Process> processes) {
    cout << "==========================================" << endl; 
    cout << "Processes in the system:" << endl;
    cout << "==========================================" << endl; 
    for (int i=0; i<processes.size(); i++) {
        cout << processes[i].toString() << endl;
    }
}

static void readDeviceList(
    string devicesFileName,
    vector<string> &ioDevices,
    vector<string> &queueDevices) {
    ifstream fin(devicesFileName.c_str());
    string line;
    if (!fin) {
        cerr << "Error reading device list" << endl;
        exit(-1);
    }

    while (getline(fin, line)) {
        trim(line);
        to_upper(line);
        if (!equals(line, "") && !starts_with(line, "#")) {
            if (starts_with(line, "I/O")) {
                line = line.substr(3);
                trim(line);
                ioDevices.push_back(line);
            }
            else if (starts_with(line, "QUEUE")) {
                line = line.substr(5);
                trim(line);
                queueDevices.push_back(line);
            }
            else {
                cerr << "Undefined device type: " << line << endl;
            }
        }
    }
}

static void readProcessList(
    string processesFileName,
    string &baselineAlgorithm,
    float &expectedSystemWTT,
    int &sysCallTime,
    int &timerInterval,
    vector<string> &processList) {
    ifstream fin(processesFileName.c_str());
    string line;

    if (!fin) {
        cerr << "Error reading process list" << endl;
        exit(-1);
    }

    do {
       getline(fin, line);
       trim(line);
    } while (equals(line, "") || starts_with(line, "#"));
 
    baselineAlgorithm = line;

    do {
       getline(fin, line);
       trim(line);
    } while (equals(line, "") || starts_with(line, "#"));
 
    expectedSystemWTT = atof(line.c_str());

    do {
       getline(fin, line);
       trim(line);
    } while (equals(line, "") || starts_with(line, "#"));
 
    sysCallTime = atoi(line.c_str());

    do {
       getline(fin, line);
       trim(line);
    } while (equals(line, "") || starts_with(line, "#"));
 
    timerInterval = atoi(line.c_str());

    while (getline(fin, line)) {
        trim(line);
        if (!equals(line, "") && !starts_with(line, "#")) {
            processList.push_back(line);
        }
    }
}

static void readProcessData(
    vector<string> processList,
    vector<Process> &processes,
    vector<string> ioDevices,
    vector<string> queueDevices,
    Kernel *osKernel) {

    Process *p;
    for (int i=0; i<processList.size(); i++) { 
        p = readProcess(processList[i], ioDevices,
                        queueDevices, osKernel);
        processes.push_back(*p);
    }
}

static Process *readProcess(
    string procFileName,
    vector<string> ioDeviceList,
    vector<string> queueDeviceList,
    Kernel *osKernel) {
    ifstream fin(procFileName.c_str());
    if (!fin) {
        cerr << "Error reading process file: " << procFileName << endl;
        exit(-1);
    }

    string procName;
    do {
       getline(fin, procName);
       trim(procName);
    } while (equals(procName, "") || starts_with(procName, "#"));

    string arrivalTime;
    do {
       getline(fin, arrivalTime);
       trim(arrivalTime);
    } while (equals(arrivalTime, "") || starts_with(arrivalTime, "#"));

    Process *p = new Process(procName, atoi(arrivalTime.c_str()));

    string line;
    while (getline(fin, line)) {
        trim(line);
        to_upper(line);
        if (!equals(line, "") && !starts_with(line, "#")) {
            ProcessOperation pOp(line, p);

            if (equals(pOp.getOpDevice(), "") 
                   || inList(pOp.getOpDevice(), ioDeviceList)
                   || inList(pOp.getOpDevice(), queueDeviceList)
                )
                p->addOp(pOp);
        }
    }

    return p;
}

static bool inList(
    string item,
    vector<string> list) {
    bool itemInList = false;
    for (int i=0; i<list.size(); i++) {
        if (equals(item, list[i])) {
            itemInList = true;
            break;
        }
    }
    
    return itemInList;
}
