#pragma once

#include <memory>
#include "objects/meta.h"

namespace tx_nodes {
class MetaSubscriber {
private:
    /* data */
public:
    MetaSubscriber(/* args */);
    ~MetaSubscriber();

    // non-copyable for all child class
    MetaSubscriber(const MetaSubscriber &) = delete;
    MetaSubscriber &operator=(const MetaSubscriber &) = delete;

    // receive meta from previous nodes
    virtual void meta_flow(std::shared_ptr<tx_objects::Meta> meta) = 0;
};

} // namespace tx_nodes
