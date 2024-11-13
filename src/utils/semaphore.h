#pragma once

#include <condition_variable>
#include <mutex>

namespace tx_utils {
// semaphore for queue/deque data structures in VideoPipe, used for producer-consumer pattern.
// it blocks the consumer thread until data has come.
class tx_semaphore {
public:
    tx_semaphore() {
        count_ = 0;
    }

    void signal() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [=] { return count_ > 0; });
        --count_;
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};
} // namespace tx_utils
