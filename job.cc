#include <algorithm>
#include <sstream>
#include <cstring>

#include "job.h"

const std::string Job::LABEL{"Job"}; // NOLINT
u32 Job::counter{0u};

Job::Job(const std::string &job_description)
        : id(counter++), command_tokens() {
    std::istringstream iss(job_description);

    std::string arrival_s;
    std::getline(iss, arrival_s, '\t');
    logger.verbose(LABEL, "Read \"" + arrival_s + "\" for arrival time.");
    i32 arrival_i;
    try {
        arrival_i = std::stoi(arrival_s);
    } catch (std::invalid_argument &) {
        arrival_i = -1; // leave this exception for the logger below.
    }
    if (arrival_i < 0) {
        arrival_i = 0;
        logger.warn(LABEL, "Illegal arrival time, using default value 0.");
    }
    arrival_u = static_cast<u32>(arrival_i);
    logger.verbose(LABEL, "Added arrival time " + std::to_string(arrival_u));

    std::string command, token;
    std::getline(iss, command, '\t');
    logger.verbose(LABEL, "Read \"" + command + "\" for command.");
    std::istringstream iss2(command);
    while (iss2 >> token) {
        auto tok_ptr = new char[token.length() + 1];
        strcpy(tok_ptr, token.c_str());
        command_tokens.push_back(tok_ptr);
        logger.verbose(LABEL, "Added token " + token);
    }
    if (command_tokens.empty()) {
        logger.warn(LABEL, "No command inputted!");
    }

    std::string duration_s;
    std::getline(iss, duration_s);
    logger.verbose(LABEL, "Read \"" + duration_s + "\" for giving_duration.");
    try {
        duration = std::stoi(duration_s);
    } catch (std::invalid_argument &) {
        duration = -1;
        logger.warn(LABEL, "Illegal giving_duration, using default value -1.");
    }
    logger.verbose(LABEL, "Added giving_duration " +
                          std::to_string(duration));
    logger.debug(LABEL, "Parsed job_discription: " + to_string());
}

// Copy constructor
Job::Job(const Job &job) : id(job.id),
                           arrival_u(job.arrival_u),
                           duration(job.duration),
                           command_tokens() {
    logger.debug(LABEL, "Copying..." + job.to_string());
    for (const auto str: job.command_tokens) {
        auto length = static_cast<u32>(strlen(str));
        auto *n_str = new char[length + 1];
        strcpy(n_str, str);
        command_tokens.push_back(n_str);
    }
    logger.debug(LABEL, "Copied: " + to_string());
}

// Destructor
Job::~Job() {
    logger.verbose(LABEL, "Destructing... " + to_string());
    std::for_each(command_tokens.begin(), command_tokens.end(),
                  [](const char *address) { delete[] address; });
}

u32 Job::get_arrival() const {
    return arrival_u;
}

i32 Job::get_duration() const {
    return duration;
}

std::vector<char *> Job::get_command_tokens() const {
    return command_tokens;
}

const std::string Job::to_string() const {
    std::ostringstream oss;
    oss << "\n\tID: " << id;
    oss << "\n\tArrival: " << arrival_u << "\n\t";
    std::for_each(command_tokens.begin(), command_tokens.end(),
                  [&](const std::string &s) { oss << s << ' '; });
    oss << "\n\tDuration: " << duration;
    return oss.str();
}

u32 Job::get_id() const {
    return id;
}

const std::string Job::command_to_string() const {
    std::ostringstream oss;
    std::for_each(command_tokens.begin(), command_tokens.end(),
                  [&](const char *token) { oss << token << ' '; });
    return oss.str();
}

Job &Job::operator=(const Job &job) {
    this->id = job.id;
    this->arrival_u = job.arrival_u;
    this->duration = job.duration;
    std::for_each(command_tokens.begin(), command_tokens.end(),
                  [](const char *str) {
                      delete[] str;
                  });
    this->command_tokens = std::vector<char *>();
    std::for_each(job.command_tokens.begin(), job.command_tokens.end(),
                  [&](const char *str) {
                      auto *dest = new char[::strlen(str) + 1];
                      ::strcpy(dest, str);
                      this->command_tokens.push_back(dest);
                  });
    return *this;
}
