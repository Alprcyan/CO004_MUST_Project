#include <algorithm>

#include "policy.h"

const std::string Policy::FCFS{"FCFS"}; // NOLINT
const std::string Policy::RR{"RR"}; // NOLINT
const std::string Policy::PRIORITY{"PRIORITY"}; // NOLINT
const std::string Policy::SJF{"SJF"}; // NOLINT

bool Policy::is_FCFS() const {
    return policy == FCFS;
}

bool Policy::is_RR() const {
    return policy == RR;
}

bool Policy::is_PRIORITY() const {
    return policy == PRIORITY;
}

bool Policy::is_SJF() const {
    return policy == SJF;
}

Policy::Policy(std::string policy) {
    std::transform(this->policy.begin(), this->policy.end(),
                  this->policy.begin(), ::toupper); // to upper case.
    this->policy = std::move(policy);
    if (is_PRIORITY() || is_SJF()) {
        state |= NOT_IMPLEMENTED;
    } else if (!(is_FCFS() || is_RR())) {
        state |= WRONG;
    }
}

std::string Policy::to_string() const {
    return policy;
}

Policy::Policy() {
    state |= WRONG;
}

bool Policy::good() const {
    return state == GOOD;
}

bool Policy::is_wrong() const {
    return state & WRONG;
}

bool Policy::is_not_implemented() const {
    return state & NOT_IMPLEMENTED;
}