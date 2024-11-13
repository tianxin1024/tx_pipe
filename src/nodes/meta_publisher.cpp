#include "meta_publisher.h"

namespace tx_nodes {

MetaPublisher::MetaPublisher(/* args */) {
}

MetaPublisher::~MetaPublisher() {
}

void MetaPublisher::add_subscriber(std::shared_ptr<MetaSubscriber> subscriber) {
    std::lock_guard<std::mutex> guard(this->subscribers_lock_);
    this->subscribers_.push_back(subscriber);
}

void MetaPublisher::remove_subscriber(std::shared_ptr<MetaSubscriber> subscriber) {
    std::lock_guard<std::mutex> guard(this->subscribers_lock_);
    for (auto i = this->subscribers_.begin(); i != this->subscribers_.end();) {
        if (*i == subscriber) {
            i = this->subscribers_.erase(i);
        } else {
            i++;
        }
    }
}

// by default, we push meta to next nodes indiscriminately, each next node has the same meta pointer.
// in some situations, we need push meta depend on condition, refer to tx_split_node which would push meta by channel index or push a deep copy pf meta(new pointer to new memory).
void MetaPublisher::push_meta(std::shared_ptr<tx_objects::Meta> meta) {
    std::lock_guard<std::mutex> guard(this->subscribers_lock_);
    for (auto i = this->subscribers_.begin(); i != this->subscribers_.end(); i++) {
        (*i)->meta_flow(meta);
    }
}
} // namespace tx_nodes
