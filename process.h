#pragma once

#include <array>
#include <chrono>
#include <memory>
#include <string>

#include "schedulerutils.h"
#include "job.h"

class Process {
public:
    using State = u32;

private:
    static constexpr useconds_t USEC_PER_SEC{1000000u};
    static constexpr State WAITING_STATE{1u};
    static constexpr State DEAD_STATE{WAITING_STATE << 2u};
    static const std::string LABEL;
    static std::array<char *, 128> ARGS_TEMP; // This TEMP array is only used when creating a
    // new process, and no two processes would be created at the same time.

    static pid_t do_job(const char *file, char *const argv[]);

    static void sleep1s();

    static void send_suspend(pid_t pid);

    static void send_cont(pid_t pid);

private:
    void update_time_point();

    i64 microseconds_since_last_update() const;

private:
    pid_t pid;
    i32 giving_duration;
    Job job_description;
    std::chrono::time_point<std::chrono::steady_clock> start_tp;
    u64 us_sum;
    u64 giving_us;
    State state;

    void cont();

    void start();

    void die();

    void run();

    void kill();

public:
    explicit Process(const Job &job) :
            pid(0),
            giving_duration(job.get_duration()),
            job_description(job),
            start_tp(),
            us_sum(0),
            giving_us(job.get_duration() > 0 ?
                      job.get_duration() *
                      USEC_PER_SEC :
                      UINT64_MAX),
            state(0) {}

    bool is_terminated();

    bool is_new();

    bool is_timeout() const;

    Job get_job() const;

    bool run1s();

    std::string to_string() const;
};
