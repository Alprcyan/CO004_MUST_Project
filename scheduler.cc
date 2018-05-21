#include <algorithm>

#include "scheduler.h"

const std::string Scheduler::LABEL{"SCHEDULER"}; // NOLINT
const std::string Scheduler::ExecuteReport::RUN{"run"}; // NOLINT
const std::string Scheduler::ExecuteReport::CONT{"continued"}; // NOLINT
const std::string Scheduler::ExecuteReport::STOP{"suspended"}; // NOLINT
const std::string Scheduler::ExecuteReport::TERM{"terminated"}; // NOLINT

void Scheduler::sleep1s() {
    usleep(one_second);
    ++sec;
}

bool Scheduler::run1s(Process &process) {
    ++sec;
    return process.run1s();
}

void Scheduler::add_process(const Job &job) {
    logger.verbose(LABEL, "Adding job_discription " + job.to_string() +
                          "\nto scheduler.");
    processes.push(Process(job));
}

bool Scheduler::empty() const {
    return processes.empty();
}

void Scheduler::run() {
    while (!processes.empty()) {
        Process process{processes.front()};
        processes.pop();
        const Job job = process.get_job();
        logger.debug(LABEL,
                     "[Sec:" + std::to_string(sec) +
                     "] Load process with job_discription " +
                     job.to_string());
        u32 arrival = job.get_arrival();
        logger.debug(LABEL,
                     "[Sec:" + std::to_string(sec) +
                     "] Read arrival time = " +
                     std::to_string(arrival));
        while (sec < arrival) {
            sleep1s();
        }
        reports.emplace_back(sec, job.get_id(),
                             process.is_new() ? ExecuteReport::RUN
                                              : ExecuteReport::CONT);
        u32 dec = quantum != 0u ? quantum : UINT32_MAX;
        logger.debug(LABEL, "Now the value of dec is: " + std::to_string(dec));
        while (true) {
            bool qua{dec-- != 0}; // whether allowed to run this term;
            logger.debug(LABEL, "The process is:" + process.to_string());
            if (!qua) { // RR: running out of quantum are not allowed
                if (processes.empty() ||
                    sec < processes.front().get_job().get_arrival()) {
                    // but if it's currently the only job left,
                    // continue running it.
                    dec += quantum;
                } else {
                    break;
                }
            }
            if (run1s(process)) {
                logger.verbose(LABEL, "Process terminated.");
                break;
            }
        }
        if (process.is_terminated()) {
            reports.emplace_back(sec, job.get_id(), ExecuteReport::TERM);
        } else {
            reports.emplace_back(sec, job.get_id(), ExecuteReport::STOP);
            processes.push(process);
        }
    }
}

std::vector<Scheduler::ExecuteReport> Scheduler::get_reports() const {
    return reports;
}

