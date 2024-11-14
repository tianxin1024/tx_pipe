#pragma once

#include <stdexcept>

namespace tx_excepts {

class InvalidCallingError : public std::runtime_error {
private:
    /* data */
public:
    InvalidCallingError(const std::string &what_arg);
    ~InvalidCallingError();
};

inline InvalidCallingError::InvalidCallingError(const std::string &what_arg) :
    std::runtime_error(what_arg) {
}

inline InvalidCallingError::~InvalidCallingError() {
}

} // namespace tx_excepts
