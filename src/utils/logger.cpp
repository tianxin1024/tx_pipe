#include "utils/logger.h"

namespace tx_utils {

Logger::Logger(/* args */) {
}

Logger::~Logger() {
    die();
    if (log_writer_th_.joinable()) {
        log_writer_th_.join();
    }
}

void Logger::die() {
    alive_ = false;
    std::lock_guard<std::mutex> guard(log_cache_mutex_);
    log_cache_.push("die");
    log_cache_semaphore_.signal();
}

void Logger::init() {
    inited_ = true;

    // initialize file writer
    file_writer_.init(log_dir_, log_file_name_template_);

    // run thread
    auto t = std::thread(&Logger::log_write_run, this);
    log_writer_th_ = std::move(t);
}

void Logger::log(LogLevel level, const std::string &message, const char *code_file, int code_line) {
    // make sure logger is initialized
    if (!inited_) {
        throw "Logger is not initialized yet!";
    }

    // level filter
    if (level > log_level_) {
        return;
    }

    // keywords filter for debug level
    if (level == LogLevel::DEBUG && keywords_for_debug_log_.size() != 0) {
        bool filterd = true;
        for (auto &keywords : keywords_for_debug_log_) {
            if (message.find(keywords) != std::string::npos) {
                filterd = false;
                break;
            }
        }
        if (filterd) {
            return;
        }
    }

    /* create log */
    std::string new_log = "";
    // 100% true for log time
    if (include_time_) {
        new_log += tx_utils::time_format(NOW, log_time_templete_);
    }

    // log level
    if (include_level_) {
        new_log += "[" + log_level_names_.at(level) + "]";
    }

    // thread id
    if (include_thread_id_) {
        auto id = std::this_thread::get_id();
        std::stringstream ss;
        ss << std::hex << id; // to hex
        auto thread_id = ss.str();
        new_log += "[" + thread_id + "]";
    }

    // code location
    if (include_code_location_) {
        new_log += "[" + std::string(code_file) + ":" + std::to_string(code_line) + "]";
    }

    new_log += " " + message;

    /* write to cache */
    // min lock range
    std::lock_guard<std::mutex> guard(log_cache_mutex_);
    log_cache_.push(new_log);
    // notify
    log_cache_semaphore_.signal();
}

void Logger::log_write_run() {
    bool log_thres_warned = false;
    /* below code runs in single thread */
    while (inited_ && alive_) {
        // wait for data
        log_cache_semaphore_.wait();
        auto log = log_cache_.front();
        log_cache_.pop();

        if (log == "die") {
            continue;
        }

        /* watch the log cache size */
        auto log_cache_size = 0;
        {
            // min lock range
            std::lock_guard<std::mutex> guard(log_cache_mutex_);
            log_cache_size = log_cache_.size();
        }
        if (!log_thres_warned && log_cache_size > log_cache_warn_threshold_) {
            TX_WARN(tx_utils::string_format("[logger] log cache size is exceeding threshold! cache size is: [%d], threshold is: [%d]", log_cache_size, log_cache_warn_threshold_));
            log_thres_warned = true; // warn 1 time
        }
        if (log_cache_size <= log_cache_warn_threshold_) {
            log_thres_warned = false;
        }

        /* write to devices */
        if (log_to_console_) {
            write_to_console(log);
        }

        if (log_to_file_) {
            write_to_file(log);
        }
    }
}

void Logger::write_to_console(const std::string &log) {
    std::cout << log << std::endl;
}

void Logger::write_to_file(const std::string &log) {
    // file_writer.write(log);
    file_writer_ << log;
}

void Logger::write_to_kafka(const std::string &log) {
}

} // namespace tx_utils
