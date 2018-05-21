#pragma once

#include <queue>
#include <utility>
#include <vector>
#include <string>
#include <utility>
#include <unistd.h>

#include "process.h"

class Scheduler {
private:
    static const std::string LABEL;

    static constexpr useconds_t one_second{1000000u};

public:
    struct ExecuteReport {
    public:
        static const std::string RUN;
        static const std::string CONT;
        static const std::string STOP;
        static const std::string TERM;

        u32 sec;
        u32 job_id;
        std::string action;

        ExecuteReport(u32 sec, u8 job_id, std::string action) :
                sec(sec), job_id(job_id), action(std::move(action)) { }
    };

private:
    std::queue<Process> processes;
    std::vector<ExecuteReport> reports;
    u32 quantum;
    u32 sec;

    void sleep1s();

    bool run1s(Process & process);

public:
    explicit Scheduler() : processes(), reports(),
                           quantum(0u), sec(0u) { }

    explicit Scheduler(u32 quantum) : processes(), reports(),
                                      quantum(quantum), sec(0u) { }

    void add_process(const Job & job);

    bool empty() const;

    void run(); // this method will block the thread.

    std::vector<ExecuteReport> get_reports() const;
};