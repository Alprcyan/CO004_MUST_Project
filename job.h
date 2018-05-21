#pragma once

#include <string>
#include <vector>

#include "schedulerutils.h"
#include "sched.h"

class Job {
private:
    static const std::string LABEL;
    static u32 counter;

private:
    u32 id;
    u32 arrival_u;
    i32 duration;
    std::vector<char *> command_tokens;

public:
    Job(const std::string & job_description); // NOLINT

    Job(const Job & job);

    ~Job();

    Job & operator=(const Job & job);

    u32 get_arrival() const;

    i32 get_duration() const;

    const std::string to_string() const;

    const std::string command_to_string() const;

    std::vector<char *> get_command_tokens() const;

    u32 get_id() const;
};
