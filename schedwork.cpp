#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<int>& shiftsUsed,
    size_t day,
    size_t workerIdx
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    sched.resize(avail.size()); // ensure sched[day] is valid

    // Add your code below

    size_t day = 0;
    size_t workerIndex = 0;
    // number of workers 
    const size_t k = avail[0].size();
    std::vector<int> shiftsUsed(k, 0);
    
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, day, workerIndex);
}

bool scheduleHelper(
    const AvailabilityMatrix& avail, 
    const size_t dailyNeed, 
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<int>& shiftsUsed,
    size_t day,
    size_t workerIndex
) {

    
    if (day == avail.size()) {
        // we have scheduled all days
        return true;
    }

    // if today is filled move to next day
    if (sched[day].size() == dailyNeed) {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, day + 1, 0);
    }

    for (size_t worker = 0; worker < avail[day].size(); worker++) {
        if (!avail[day][worker]) continue;
        if (shiftsUsed[worker] >= static_cast<int>(maxShifts)) continue;
        // worker has already been scheduled
        if (std::find(sched[day].begin(), sched[day].end(), worker) != sched[day].end()) continue; 

        // insert worker to try
        sched[day].push_back(worker);
        shiftsUsed[worker]++;

        if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, day, workerIndex + 1)) {
            return true;
        }

        // try backtrack if worker doesn't work
        sched[day].pop_back();
        shiftsUsed[worker]--;
    }

    return false;

}

