#include "meta.h"

namespace tx_objects {

Meta::Meta(MetaType meta_type, int channel_index) :
    meta_type_(meta_type),
    channel_index_(channel_index) {
    create_time_ = std::chrono::system_clock::now();
}

Meta::~Meta() {
}

std::string Meta::get_traces_str() {
    return "";
}

std::string Meta::get_meta_str() {
    return "";
}

} // namespace tx_objects
