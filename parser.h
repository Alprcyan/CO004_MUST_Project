/**
 *
 */
#pragma once

#include <array>
#include <string>

#include "schedulerutils.h"
#include "policy.h"


class Parser {
public:
    using State = u32;

private:
    static const std::string LABEL;
    static const std::string DEFAULT_FILENAME;
    static constexpr State GOOD{0u};

public:
    static constexpr State WRONG_POLICY{1u};
    static constexpr State NO_POLICY{WRONG_POLICY << 1u};
    static constexpr State TOO_MANY_ARGS{NO_POLICY << 1u};

private:
    State state;
    std::string filename;
    Policy policy;
    u32 quantum;

public:
    Parser(u32 argc, char * args[]);

    bool good() const;

    State get_state() const;

    std::string get_filename() const;

    Policy get_policy() const;

    u32 get_quantum() const;
};
