#pragma once

#include <iostream>
#include <ostream>
#include <map>
#include <string>

// #define ENABLE_LOG

#ifdef ENABLE_LOG
#define LOG_WTF
#define LOG_ERROR
#define LOG_WARN
#define LOG_INFO
#define LOG_VERBOSE
#define LOG_DEBUG
#endif

class Logger {
private:
    static const std::string LABEL;

private:
    std::ostream & wtf_os;
    std::ostream & error_os;
    std::ostream & warn_os;
    std::ostream & info_os;
    std::ostream & verbose_os;
    std::ostream & debug_os;

    void output(std::ostream & os, const std::string & kind,
                const std::string & label, const std::string & message) const;

    void split_line_output(std::ostream & os,
                           const std::string & kind,
                           const std::string & label,
                           const std::string & message) const;

public:
    Logger(std::ostream & wtf_os, std::ostream & err_os,
           std::ostream & warn_os, std::ostream & info_os,
           std::ostream & verbose_os, std::ostream & debug_os) :
            wtf_os(wtf_os), error_os(err_os),
            warn_os(warn_os), info_os(info_os),
            verbose_os(verbose_os), debug_os(debug_os) { }

    Logger(std::ostream & err_os, std::ostream & noncritical_os) :
            Logger(err_os, err_os, noncritical_os,
                   noncritical_os, noncritical_os, noncritical_os) { }

    Logger() : Logger(std::cerr, std::cout) { }

    void debug(const std::string & label, const std::string & message) const;

    void verbose(const std::string & label, const std::string & message) const;

    void info(const std::string & label, const std::string & message) const;

    void warn(const std::string & label, const std::string & message) const;

    void error(const std::string & label, const std::string & message) const;

    void wtf(const std::string & label, const std::string & message) const;
};
