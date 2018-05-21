#include <ctime>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <algorithm>

#include "process.h"

const std::string Process::LABEL{"Process"}; // NOLINT
std::array<char *, 128> Process::ARGS_TEMP{ };

/**
 * Make this method static or fork() wont work well.
 */
pid_t Process::do_job(const char * file, char * const argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(file, argv);
    } else {
        // the child process won't reach this line.
        sleep1s();
        send_suspend(pid);
    }
    return pid;
}

void Process::sleep1s() {
    usleep(USEC_PER_SEC);
}

void Process::send_suspend(pid_t pid) {
    ::kill(pid, SIGTSTP);
}

void Process::send_cont(pid_t pid) {
    ::kill(pid, SIGCONT);
}

void Process::update_time_point() {
    this->start_tp = std::chrono::steady_clock::now();
}

i64 Process::microseconds_since_last_update() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - start_tp).count();
}

void Process::start() {
    std::vector<char *> tokens = job_description.get_command_tokens();
    usize i = 0;
    std::for_each(tokens.begin(), tokens.end(), [&](const char *) {
        ARGS_TEMP[i] = tokens[i];
        ++i;
    });
    ARGS_TEMP[i] = NULL;
    logger.verbose(LABEL,
                   std::string("Creating a new process with job: ") +
                   job_description.to_string());
    update_time_point();
    pid_t pid{do_job(ARGS_TEMP[0], &ARGS_TEMP[0])};
    us_sum += microseconds_since_last_update();
    state = WAITING_STATE;
    this->pid = pid;
    logger.verbose(LABEL,
                   "Process started with pid: " +
                   std::to_string(pid));
}

void Process::die() {
    state = DEAD_STATE;
    pid = -1;
}

// continue the process from suspend.
void Process::cont() {
    if (state == WAITING_STATE) {
        update_time_point();
        send_cont(pid);
        sleep1s();
        send_suspend(pid);
        us_sum += microseconds_since_last_update();
        logger.verbose(LABEL,
                       "Executed process " + std::to_string(this->pid) +
                       " for one more second.");
    } else {
        logger.warn(LABEL, "Trying to continue process " + std::to_string(pid) +
                           " which isn't waiting.");
    }
}

// if the process is or was terminated, return true.
// otherwise return false.
bool Process::is_terminated() {
    if (this->state == DEAD_STATE) { // terminated
        logger.debug(LABEL, "Asking a terminated process if it's terminated.");
        return true;
    } else if (this->pid != 0 && // not zero
               waitpid(this->pid, nullptr, WNOHANG) != 0) {
        logger.verbose(LABEL, "Process " + std::to_string(this->pid) +
                              " is terminated.");
        die();
        return true;
    }
    return false;
}

void Process::kill() {
    ::kill(pid, SIGKILL);
    us_sum += microseconds_since_last_update();
    logger.verbose(LABEL, "Killing process " + std::to_string(pid));
    die();
}

bool Process::is_timeout() const {
    return giving_duration != -1 && giving_us < us_sum;
}

void Process::run() {
    if (pid != -1) {
        if (pid == 0) {
            start();
        } else {
            cont();
        }
    }
}

bool Process::is_new() {
    return pid == 0;
}

Job Process::get_job() const {
    return job_description;
}

std::string Process::to_string() const {
    return std::to_string(pid) + job_description.to_string();
}

bool Process::run1s() {
    run();
    if (is_timeout()) {
        kill();
    }
    return is_terminated();
}
