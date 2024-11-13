#pragma once

#include <string>
#include <fstream>
#include <chrono>
#include <experimental/filesystem>

#include "utils/utils.h"

namespace tx_utils {
// log file writer, write log to local file. auto create new log file bye date.
// NOT thread safe, MUST use in single thread.
class LogFileWriter {
private:
    // ready to go
    bool inited_ = false;

    // root dir for log saving
    std::string log_dir_;
    // log file name template, such as `<year>-<mon>-<day>.txt`, refer to tx_utils::time_format(...) for rule details
    std::string log_file_name_template_;

    // file handle
    std::ofstream log_writer_;
    // get valid log file name including path, name and extension
    std::string create_valid_log_file_name();

    // current log day (1 ~ 31)
    int log_day_ = 0;

    // get day of now
    int get_now_day();

#define NOW std::chrono::system_clock::now()
public:
    LogFileWriter();
    ~LogFileWriter();

    // write log
    void write(std::string log);

    // initialize writer
    void init(std::string log_dir, std::string log_file_name_template);

    // for << operator
    LogFileWriter &operator<<(std::string log);
};

} // namespace tx_utils
