#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <regex>

// https://github.com/HowardHinnant/date/blob/master/include/date/date.h
#include "date.h"

using namespace std;
using namespace std::chrono;

namespace tx_utils {

// string format in C++17
template <typename... Args>
inline string string_format(const string &format, Args... args) {
    size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args...);
    // unique_ptr<char[]> buf(new char[size]);
    char bytes[size];
    snprintf(bytes, size, format.c_str(), args...);
    return string(bytes);
}

// split time point to 7 parts, include year, month, day ... in order
inline void time_split(system_clock::time_point tp, std::vector<int> &time_parts, int time_zone = 8) {
    // right time zone
    tp = tp + std::chrono::hours{time_zone};

    auto dp = date::floor<date::days>(tp);

    auto ymd = date::year_month_day{dp};
    auto time = date::make_time(std::chrono::duration_cast<std::chrono::milliseconds>(tp - dp));

    time_parts.clear();

    // push year/month/day
    time_parts.push_back(static_cast<int>(ymd.year()));
    time_parts.push_back(static_cast<unsigned>(ymd.month()));
    time_parts.push_back(static_cast<unsigned>(ymd.day()));

    // push hour/minute/second/milisecond
    time_parts.push_back(time.hours().count());
    time_parts.push_back(time.minutes().count());
    time_parts.push_back(time.seconds().count());
    time_parts.push_back(time.subseconds().count());
}

// set width and fill, like ostream.fill() and ostream.width()
inline std::string set_width_and_fill(std::string str,
                                      int width,
                                      char fill = '0',
                                      bool fill_left = true) {
    while (str.size() < width) {
        if (fill_left) {
            str = fill + str;
        } else {
            str = str + fill;
        }
    }
    return str;
}

// format time point to string, chars in <> are keywords.
// in: <year>-<mon>-<day> <hour>:<min>:<sec>.<mili>
// out: 2022-10-08 13:53:07.230
// or
// in: [<day>/<mon>/<year> <hour>:<min>:<sec>.<mili>]
// out: [08/10/2022 13:53:07.230]
inline std::string time_format(system_clock::time_point tp, std::string template_str = "<year>-<mon>-<day> <hour>:<min>:<sec>.<mili>", int time_zone = 8) {
    // right time zone
    tp = tp + std::chrono::hours{time_zone};

    // 7 parts
    std::vector<int> time_parts;
    time_split(tp, time_parts, 0);

    auto time_str = std::regex_replace(template_str, std::regex("<year>"), std::to_string(time_parts[0]));
    time_str = std::regex_replace(time_str, std::regex("<mon>"), set_width_and_fill(std::to_string(time_parts[1]), 2));
    time_str = std::regex_replace(time_str, std::regex("<day>"), set_width_and_fill(std::to_string(time_parts[2]), 2));
    time_str = std::regex_replace(time_str, std::regex("<hour>"), set_width_and_fill(std::to_string(time_parts[3]), 2));
    time_str = std::regex_replace(time_str, std::regex("<min>"), set_width_and_fill(std::to_string(time_parts[4]), 2));
    time_str = std::regex_replace(time_str, std::regex("<sec>"), set_width_and_fill(std::to_string(time_parts[5]), 2));
    time_str = std::regex_replace(time_str, std::regex("<mili>"), set_width_and_fill(std::to_string(time_parts[6]), 3));

    return time_str;
}

} // namespace tx_utils
