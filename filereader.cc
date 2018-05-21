#include <algorithm>
#include <sstream>
#include <unistd.h>

#include "filereader.h"

const std::string FileReader::LABEL{ "FileReader" }; // NOLINT

FileReader::FileReader(const std::string & filename) {
    this->state = FileReader::GOOD;
    this->filename = filename;
    if (file_exist()) {
        std::ifstream ifs(filename, std::ifstream::in);
        logger.verbose(LABEL, "Opening file " + this->filename);
        std::string line;
        while (getline(ifs, line)) {
            logger.verbose(LABEL, "Read: " + line);
            lines.push_back(line);
        }
        ifs.close();
        logger.verbose(LABEL, "Closing file.");
        current_line = lines.begin();
    } else {
        this->state |= FILE_NOT_FOUND;
        logger.error(LABEL, "File " + this->filename + " not exist!");
    }

    if (good()) {
        logger.info(LABEL, "FileReader succeed:\n"
                           "\tLoad jobs: " +
                           std::to_string(this->lines.size()));
    }
}

bool FileReader::file_exist() const {
    return access(this->filename.c_str(), F_OK) != -1;
}

std::vector<std::string> FileReader::to_vector() const {
    return this->lines;
}

std::string FileReader::get_name() const {
    return this->filename;
}

FileReader::State FileReader::get_state() const {
    return this->state;
}

bool FileReader::good() const {
    return this->state == FileReader::GOOD;
}
