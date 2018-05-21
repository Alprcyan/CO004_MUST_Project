#include <array>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <chrono>

#include "logger.h"
#include "schedulerutils.h"

const std::string Logger::LABEL{"Logger"}; // NOLINT

void Logger::output(std::ostream & os,
                    const std::string & kind,
                    const std::string & label,
                    const std::string & message) const {
#ifdef ENABLE_LOG
    auto tim = time(nullptr);
    os << '['
       << std::put_time(std::gmtime(&tim), "%c")
       << "]["
       << label << "]["
       << kind << "]: "
       << message
       << std::endl;
#endif
}

void Logger::split_line_output(std::ostream & os,
                               const std::string & kind,
                               const std::string & label,
                               const std::string & message) const {
#ifdef ENABLE_LOG
    std::istringstream iss(message);
    std::string line;
    while (getline(iss, line)) {
        output(os, kind, label, line);
    }
#endif
}

void Logger::verbose(const std::string & label,
                     const std::string & message) const {
#ifdef LOG_VERBOSE
    split_line_output(verbose_os, "VERBOSE", label, message);
#endif
}

void Logger::info(const std::string & label,
                  const std::string & message) const {
#ifdef LOG_INFO
    split_line_output(verbose_os, "INFO", label, message);
#endif
}

void Logger::warn(const std::string & label,
                  const std::string & message) const {
#ifdef LOG_WARN
    split_line_output(verbose_os, "WARN", label, message);
#endif
}

void Logger::error(const std::string & label,
                   const std::string & message) const {
#ifdef LOG_ERROR
    split_line_output(verbose_os, "ERROR", label, message);
#endif
}

void Logger::wtf(const std::string & label,
                 const std::string & message) const {
#ifdef LOG_WTF
    split_line_output(verbose_os, "WTF", label, message);
#endif
}

void Logger::debug(const std::string & label,
                   const std::string & message) const {
#ifdef LOG_DEBUG
    split_line_output(debug_os, "DEBUG", label, message);
#endif
}
