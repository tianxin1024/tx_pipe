#pragma once

#include <memory>
#include <vector>
#include <mutex>
#include <iostream>

#include "meta_subscriber.h"

namespace tx_nodes {

class MetaPublisher {
private:
protected:
    // push meta to next nodes
    virtual void push_meta(std::shared_ptr<tx_objects::Meta> meta);

    // non-copyable for all child class
    std::mutex subscribers_lock_;
    // next nodes as subscribers
    std::vector<std::shared_ptr<MetaSubscriber>> subscribers_;

public:
    MetaPublisher(/* args */);
    ~MetaPublisher();

    // add next node
    void add_subscriber(std::shared_ptr<MetaSubscriber> subscriber);
    // remove next node
    void remove_subscriber(std::shared_ptr<MetaSubscriber> subscriber);
};

} // namespace tx_nodes
