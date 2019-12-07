//tanner hermann


#include "process.h"


int RoundRobin(const int& curTime, const vector<Process>& procList, const int& timeQuantum)
{

    static int timeToNextSched = timeQuantum; // keep track of when next process to be scheduled
    static deque<int> ready; // keep track of process that can be scheduled

    int idx = -1, chk;
    bool done;

    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
        }
    }

    if(timeToNextSched == 0 || procList[ready[0]].isDone)
    {
        if(!procList[ready[0]].isDone)
        {
            ready.push_back(ready[0]);
        }

        //remove the process from the front of the ready queue and reset time until next scheduling
        ready.pop_front();
        timeToNextSched = timeQuantum;
    }

    if(ready.size() > 0)
    {
        idx = ready[0];
        --timeToNextSched;

    }
    else
    {
        idx = -1;
        timeToNextSched = 0;
    }

    return idx;
}