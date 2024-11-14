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

class InvalidArgumentError : public std::runtime_error {
private:
    /* data */
public:
    InvalidArgumentError(const std::string &what_arg);
    ~InvalidArgumentError();
};

inline InvalidArgumentError::InvalidArgumentError(const std::string &what_arg) :
    std::runtime_error(what_arg) {
}

inline InvalidArgumentError::~InvalidArgumentError() {
}

class InvalidPipelineError : public std::runtime_error {
private:
    /* data */
public:
    InvalidPipelineError(const std::string &what_arg);
    ~InvalidPipelineError();
};

inline InvalidPipelineError::InvalidPipelineError(const std::string &what_arg) :
    std::runtime_error(what_arg) {
}

inline InvalidPipelineError::~InvalidPipelineError() {
}

// not implemented error
class NotImplementedError : public std::runtime_error {
private:
    /* data */
public:
    NotImplementedError(const std::string &what_arg);
    ~NotImplementedError();
};

inline NotImplementedError::NotImplementedError(const std::string &what_arg) :
    std::runtime_error(what_arg) {
}

inline NotImplementedError::~NotImplementedError() {
}

} // namespace tx_excepts
