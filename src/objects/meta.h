#pragma once

#include <string>
#include <map>
#include <any>
#include <vector>
#include <memory>
#include <chrono>
#include <assert.h>

namespace tx_objects {

// meta type
enum MetaType {
    FRAME,
    CONTROL
};

// meta trace field
// 1. sequence   ->int       ,sequence number the meta flowing through pipeline
// 2. node_name  ->string    ,name of current node the meta flow through
// 3. in_time    ->long      ,time when the meta arrive current node
// 4. out_time   ->long      ,time when the meta leave current node
// 5. text_info  ->vector    ,text info while the meta inside node
enum MetaTraceField {
    SEQUENCE,
    NODE_NAME,
    IN_TIME,
    OUT_TIME,
    TEXT_INFO
};

// base class for meta
class Meta {
private:
protected:
public:
    Meta(MetaType meta_type, int channel_index);
    ~Meta();

    // the time when meta created
    std::chrono::system_clock::time_point create_time_;

    MetaType meta_type_;

    // channel the meta belongs to
    int channel_index_;

    // write trace record or not
    bool trace_on_ = false;

    // format traces string
    virtual std::string get_traces_str();

    // format meta string
    virtual std::string get_meta_str();

    // virtual clone method since we do not know what specific meta we need copy in some situations, return a new pointer pointting to new memory allocation in heap.
    // note: every child class need implement its own clone() method.
    virtual std::shared_ptr<Meta> clone() = 0;
};

} // namespace tx_objects
