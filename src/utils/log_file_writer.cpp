#include "log_file_writer.h"

namespace tx_utils {

LogFileWriter::LogFileWriter() {
}

LogFileWriter::~LogFileWriter() {
    if (log_writer_.is_open()) {
        log_writer_.close();
    }
}

void LogFileWriter::init(std::string log_dir, std::string log_file_name_template) {
    this->log_dir_ = log_dir;
    this->log_file_name_template_ = log_file_name_template;

    // open log file first time
    auto f = create_valid_log_file_name();
    log_writer_.open(f, std::ofstream::out | std::ofstream::app);

    inited_ = true;
}

void LogFileWriter::write(std::string log) {
    if (!inited_) {
        throw "tx_log_file_writer not initialized!";
    }

    // check if need create new log file
    if (get_now_day() != log_day_) {
        if (log_writer_.is_open()) {
            log_writer_.close();
        }

        auto f = create_valid_log_file_name();
        log_writer_.open(f, std::ofstream::out | std::ofstream::app);
    }

    log_writer_ << log << std::endl;
}

std::string LogFileWriter::create_valid_log_file_name() {
    if (!std::experimental::filesystem::exists(log_dir_)) {
        std::experimental::filesystem::create_directories(log_dir_);
    }
    std::experimental::filesystem::path root_dir(log_dir_);

    auto f_name = tx_utils::time_format(NOW, log_file_name_template_);
    auto p = root_dir / f_name;

    // cache log start day
    log_day_ = get_now_day();

    return p.string();
}

int LogFileWriter::get_now_day() {
    std::vector<int> time_parts;
    tx_utils::time_split(NOW, time_parts);

    // refer to tx_utils::time_split(...), indice 2 is day
    return time_parts[2];
}

LogFileWriter &LogFileWriter::operator<<(std::string log) {
    write(log);
    return *this;
}
} // namespace tx_utils
