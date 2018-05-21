#pragma once

#include <string>
#include <utility>
#include "schedulerutils.h"

class Policy {
private:
    static const std::string LABEL;

    static const std::string FCFS;
    static const std::string RR;
    static const std::string PRIORITY;
    static const std::string SJF;

    using State = u8;

    static constexpr State GOOD{0u};
    static constexpr State WRONG{1u};
    static constexpr State NOT_IMPLEMENTED{WRONG << 1u};

private:
    State state{GOOD}; // good;

    std::string policy;

public:
    Policy();

    explicit Policy(std::string policy);

    bool good() const;

    bool is_wrong() const;

    bool is_not_implemented() const;

    bool is_FCFS() const;

    bool is_RR() const;

    bool is_PRIORITY() const;

    bool is_SJF() const;

    std::string to_string() const;
};


