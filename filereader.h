#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "schedulerutils.h"

class FileReader {
public:
    using State = u32;

private:
    static const std::string LABEL;
    static constexpr State GOOD{ 0u};

public:
    static constexpr State FILE_NOT_FOUND{1u};

private:
    State state;
    std::string filename;
    std::vector<std::string> lines;
    std::vector<std::string>::iterator current_line;

    bool file_exist() const;

public:
    explicit FileReader(const std::string & filename);

    std::vector<std::string> to_vector() const;

    std::string get_name() const;

    State get_state() const;

    bool good() const;
};
