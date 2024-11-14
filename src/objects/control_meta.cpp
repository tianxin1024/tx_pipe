#include "objects/control_meta.h"

namespace tx_objects {

ControlMeta::ControlMeta(ControlType control_type, int channel_index, std::string control_uid) :
    Meta(MetaType::CONTROL, channel_index), control_type_(control_type), control_uid_(control_uid) {
    if (control_uid.empty()) {
        generate_uid();
    }
}

ControlMeta::~ControlMeta() {
}

std::shared_ptr<Meta> ControlMeta::clone() {
    // just call copy constructor and return new pointer
    return std::make_shared<ControlMeta>(*this);
}

void ControlMeta::generate_uid() {
    auto now = std::chrono::system_clock::now();
    auto period = now.time_since_epoch();

    // milliseconds since 1970-1-1 00:00:00
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(period).count();

    control_uid_ = "control_" + std::to_string(timestamp);
}
} // namespace tx_objects
