#pragma once

#include <chrono>
#include "objects/meta.h"

namespace tx_objects {

// type of control meta
enum ControlType {
    SPEAK,
    VIDEO_RECORD,
    IMAGE_RECORD
};

// control meta, which contains control data.
class ControlMeta : public Meta {
private:
    // help to generate control uid if need
    void generate_uid();

public:
    ControlMeta(ControlType control_type, int channel_index, std::string control_uid = "");
    ~ControlMeta();

    ControlType control_type_;
    // unique id to identify control meta (caould be generated in random)
    std::string control_uid_;

    // copy myself
    virtual std::shared_ptr<Meta> clone() override;
};

} // namespace tx_objects
