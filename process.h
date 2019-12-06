//tanner hermann

#ifndef PROCESS_H
//tanner hermann
#define PROCESS_H

#include<vector>
#include<deque>
#include<iostream>

using namespace std;



struct Process
{
    Process() : isDone(false), totalTimeNeeded(-1), timeScheduled(0), startTime(-1), timeFinished(-1) {}

    string id;
    int startTime;
    int totalTimeNeeded;

    bool isDone;
    int timeScheduled;
    int timeFinished;
};

struct ProcessHist
{
    ProcessHist() : time(0) {}

    string id;
    int time;

};

int RoundRobin(const int& curTime, const vector<Process>& procList, const int& timeQuantum);

#endif