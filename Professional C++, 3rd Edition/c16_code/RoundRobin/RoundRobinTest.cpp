#include "RoundRobin.h"
#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;

// Simple Process class.
class Process
{
public:
    // Constructor accepting the name of the process.
    Process(const string& name) : mName(name) {}

    // Implementation of doWorkDuringTimeSlice would let the process
    // perform its work for the duration of a time slice.
    // Actual implementation omitted.
    void doWorkDuringTimeSlice()
    {
        cout << "Process " << mName
             << " performing work during time slice." << endl;
    }

    // Needed for the RoundRobin::remove method to work.
    bool operator==(const Process& rhs)
    {
        return mName == rhs.mName;
    }

private:
    string mName;
};

// Simple round-robin based process scheduler.
class Scheduler
{
public:
    // Constructor takes a vector of processes.
    Scheduler(const vector<Process>& processes);

    // Selects the next process using a round-robin scheduling
    // algorithm and allows it to perform some work during
    // this time slice.
    void scheduleTimeSlice();

    // Removes the given process from the list of processes.
    void removeProcess(const Process& process);

private:
    RoundRobin<Process> rr;
};

Scheduler::Scheduler(const vector<Process>& processes)
{
    // Add the processes
    for (auto& process : processes) {
        rr.add(process);
    }

    // If your compiler does not support the above C++11
    // range-based for loop, you need the following instead:
    //for (auto it = processes.cbegin(); it != processes.cend(); ++it) {
    //  rr.add(*it);
    //}
}

void Scheduler::scheduleTimeSlice()
{
    try {
        rr.getNext().doWorkDuringTimeSlice();
    } catch (const out_of_range&) {
        cerr << "No more processes to schedule." << endl;
    }
}

void Scheduler::removeProcess(const Process& process)
{
    rr.remove(process);
}

int main()
{
    vector<Process> processes = { Process("1"), Process("2"), Process("3") };
    // If your compiler does not support the above uniform
    // initialization from C++11, you need the following:
    //vector<Process> processes;
    //processes.push_back(Process("1"));
    //processes.push_back(Process("2"));
    //processes.push_back(Process("3"));
    Scheduler sched(processes);

    for (int i = 0; i < 4; ++i)
        sched.scheduleTimeSlice();

    sched.removeProcess(processes[1]);
    cout << "Removed second process" << endl;

    for (int i = 0; i < 4; ++i)
        sched.scheduleTimeSlice();

    return 0;
}
