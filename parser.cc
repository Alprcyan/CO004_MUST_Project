#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <sys/stat.h>
#include <vector>

#include "parser.h"

const std::string Parser::LABEL{"Parser"}; // NOLINT
const std::string Parser::DEFAULT_FILENAME{"Job.txt"}; // NOLINT

Parser::Parser(u32 argc, char *args[]) : policy(args[0]), quantum(2) {
    logger.verbose(LABEL, std::string("Received ") + std::to_string(argc) +
                          " arguments");
    this->state = Parser::GOOD;
    if (argc == 0) {
        this->state |= NO_POLICY;
    } else if (argc == 1 ||
               (argc == 2 && std::all_of(args[1], args[1] + strlen(args[1]),
                                         [](int _c) -> bool {
                                             return isdigit(_c) || _c == '-';
                                         }))) {
        this->filename = Parser::DEFAULT_FILENAME;
        logger.verbose(LABEL, "Input file not specified. "
                              "Using default value Job.txt.");
        logger.verbose(LABEL, "Using args[0]=" + this->policy.to_string() +
                              " as policy.");
        if (this->policy.good()) {
            if (this->policy.is_RR()) {
                if (argc == 1) {
                    logger.warn(LABEL, "Quantum not specified. "
                                       "Using default value 2.");
                } else {
                    i32 q = atoi(args[1]); // NOLINT
                    logger.verbose(LABEL, "Using args[1]=" + std::to_string(q) +
                                          " as quantum.");
                    if (q < 1) { // not an positive value.
                        logger.warn(LABEL, "Not a valid quantum value. "
                                           "Using default value 2.");
                    } else { // Legit
                        this->quantum = static_cast<u32>(q);
                    }
                }
            } // nothing else.
        } else {
            this->state |= WRONG_POLICY;
        }
    } else if (argc == 2 ||
               (argc == 3 && std::all_of(args[2], args[2] + strlen(args[2]),
                                         [](int _c) -> bool {
                                             return isdigit(_c) || _c == '-';
                                         }))) {
        this->filename = args[0];
        logger.verbose(LABEL,
                       "Using args[0]=" + this->filename + " as filename.");
        this->policy = Policy(args[1]);
        logger.verbose(LABEL, "Using args[1]=" + this->policy.to_string() +
                              " as policy.");
        if (this->policy.good()) {
            if (policy.is_RR()) {
                if (argc == 2) {
                    logger.warn(LABEL, "Quantum not specified. "
                                       "Using default value 2.");
                } else { // argc == 3 and the third arg is number.
                    i32 q = atoi(args[2]); // NOLINT
                    logger.verbose(LABEL, "Using args[2]=" + std::to_string(q) +
                                          " as quantum.");
                    if (q < 1) { // not an positive value.
                        logger.warn(LABEL, "Not a valid quantum value. "
                                           "Using default value 2.");
                    } else { // Legit
                        this->quantum = static_cast<u32>(q);
                    }
                }
            } // nothing else
        } else {
            this->state |= WRONG_POLICY;
        }
    } else {
        this->state |= TOO_MANY_ARGS;
    }

    if (good()) { // Everything is fine.
        logger.info(LABEL, "Arguments read. Everything set.");
        std::ostringstream field_info;
        field_info << "\n\tFilename: " << this->filename
                   << "\n\tPolicy: " << this->policy.to_string();
        if (this->policy.is_RR()) {
            field_info << "\n\tQuantum: " << std::to_string(this->quantum);
        }
        logger.info(LABEL, "Arguments:" + field_info.str());
    } else {
        if (this->state & NO_POLICY) {
            logger.error(LABEL, "Expecting 2 or 3 arguments.");
        }
        if (this->state & WRONG_POLICY) {
            logger.error(LABEL,
                         this->policy.to_string() + " is not a valid policy!");
        }
        if (this->state & TOO_MANY_ARGS) {
            logger.error(LABEL, "Too many args. Excepting less than 3 or "
                                "3 if the policy is RR.");
        }
    }
}

bool Parser::good() const {
    return this->state == Parser::GOOD;
}

Parser::State Parser::get_state() const {
    return this->state;
}

std::string Parser::get_filename() const {
    return this->filename;
}

Policy Parser::get_policy() const {
    return this->policy;
}

u32 Parser::get_quantum() const {
    return this->quantum;
}
