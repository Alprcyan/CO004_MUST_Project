#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

#include "filereader.h"
#include "job.h"
#include "logger.h"
#include "parser.h"
#include "scheduler.h"
#include "schedulerutils.h"

#define PHASE_1
#define PHASE_2

Logger logger; // NOLINT. Edit logger.h to en/disable log.

const std::string OUTPUT_FILE{"JobOut.txt"}; // NOLINT

i32 main(i32 argc, char *argv[]) {
    const std::string LABEL{"Main"};
    logger.verbose(LABEL, "Program initialized.");

    std::ostringstream cmd_args;
    for (u32 i = 0; i != static_cast<u32>(argc); ++i) {
        cmd_args << "\targs[" << i << "]: " << argv[i] << std::endl;
    }
    logger.verbose(LABEL, "Arguments from commandline:\n" + cmd_args.str());

    Parser parser(static_cast<u32>(argc - 1), argv + 1);
    auto policy = parser.get_policy();

    if (parser.good()) {
        FileReader reader(parser.get_filename());
        std::vector<Job> jobs;
        if (reader.good()) {
            for (const auto &line: reader.to_vector()) {
                jobs.emplace_back(line);
            }
        }
        std::sort(jobs.begin(), jobs.end(),
                  [](const Job &a, const Job &b) -> bool {
                      return a.get_arrival() < b.get_arrival();
                  });

#ifdef PHASE_1
        u64 cmd_len = 7; // length of "Command"
        std::vector<std::string> cmds;
        for (const auto &job_discription: jobs) {
            std::string cmd = job_discription.command_to_string();
            if (cmd_len < cmd.length()) {
                cmd_len = cmd.length();
            }
            cmds.push_back(cmd);
        }
        cmd_len += 7; // make sure there're 2 spaces after cmd.

        auto str_to_len = [](const std::string &str,
                             u64 goal_len) -> std::string {
            std::ostringstream oss;
            oss << str;
            if (str.length() < goal_len) {
                oss << std::string(goal_len - str.length(), ' ');
            }
            return oss.str();
        };

        u32 jid_len = 12u;
        u32 arr_len = 16u;

        logger.verbose(LABEL, "Printing job_discription list.");
        std::cout << str_to_len("Job No.", jid_len)
                  << str_to_len("Arrival time", arr_len)
                  << str_to_len("Command", cmd_len)
                  << "Duration" << std::endl;

        for (const auto &job_discription: jobs) {
            std::cout.width(jid_len);
            std::cout << std::left << job_discription.get_id() << std::right;
            std::cout.width(arr_len);
            std::cout << std::left << job_discription.get_arrival()
                      << std::right;
            std::cout.width(0);
            std::cout
                    << str_to_len(job_discription.command_to_string(), cmd_len)
                    << job_discription.get_duration()
                    << std::endl;
        }
#endif

#ifdef PHASE_2
        auto scheduler = parser.get_policy().is_FCFS()
                         ? Scheduler()
                         : Scheduler(parser.get_quantum());
        std::for_each(jobs.begin(), jobs.end(),
                      [&scheduler](const Job &job) {
                          scheduler.add_process(job);
                      });

        scheduler.run();

        const auto &reports = scheduler.get_reports();

        std::ofstream os(OUTPUT_FILE, std::ofstream::out);
        for (const auto &report: reports) {
            os << report.sec << " sec: Job "
               << report.job_id << " "
               << report.action
               << std::endl;
        }
#endif
    } else {
        return EXIT_FAILURE;
    }
    return 0;
}
