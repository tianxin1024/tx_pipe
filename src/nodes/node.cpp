#include "nodes/node.h"

namespace tx_nodes {

Node::Node(std::string node_name) :
    node_name_(node_name) {
}

Node::~Node() {
}

// there is only one thread poping data from the in_queue, we don't use lock here when poping.
// there is only one thread pushing data to the out_queue, we don't use lock here when pushing.
void Node::handle_run() {
    // cache for batch handling if need
    std::vector<std::shared_ptr<tx_objects::FrameMeta>> frame_meta_batch_cache;
    while (alive_) {
        // wait for producer, make sure in_queue is not empty.
        this->in_queue_semaphore_.wait();

        TX_DEBUG(tx_utils::string_format("[%s] before handling meta, in_queue.size()==>%d", node_name_.c_str(), in_queue_.size()));
        auto in_meta = this->in_queue_.front();

        // dead flag
        if (in_meta == nullptr) {
            continue;
        }

        // handling hooker activated if need
        invoke_meta_handling_hooker(node_name_, in_queue_.size(), in_meta);

        std::shared_ptr<tx_objects::Meta> out_meta;
        auto batch_complete = false;

        // call handlers
        if (in_meta->meta_type_ == tx_objects::MetaType::CONTROL) {
            auto meta_2_handle = std::dynamic_pointer_cast<tx_objects::ControlMeta>(in_meta);
            out_meta = this->handle_control_meta(meta_2_handle);
        } else if (in_meta->meta_type_ == tx_objects::MetaType::FRAME) {
            auto meta_2_handle = std::dynamic_pointer_cast<tx_objects::FrameMeta>(in_meta);
            // one by one
            if (frame_meta_handle_batch_ == 1) {
                out_meta = this->handle_frame_meta(meta_2_handle);
            } else {
                // batch by batch
                frame_meta_batch_cache.push_back(meta_2_handle);
                if (frame_meta_batch_cache.size() >= frame_meta_handle_batch_) {
                    // cache complete
                    this->handle_frame_meta(frame_meta_batch_cache);
                    batch_complete = true;
                } else {
                    // cache not complete, do nothing
                    TX_DEBUG(tx_utils::string_format("[%s] handle meta with batch, frame_meta_batch_cache.size()==>%d", node_name_.c_str(), frame_meta_batch_cache.size()));
                }
            }
        } else {
            throw "invalid meta type!";
        }
        this->in_queue_.pop();
        TX_DEBUG(tx_utils::string_format("[%s] after handling meta, in_queue.size()==>%d", node_name_.c_str(), in_queue_.size()));

        // one by one mode
        // return nullptr means do not push it to next nodes(such as in des nodes).
        if (out_meta != nullptr && node_type() != NodeType::DES) {
            TX_DEBUG(tx_utils::string_format("[%s] before handling meta, out_queue.size()==>%d", node_name_.c_str(), out_queue_.size()));
            this->out_queue_.push(out_meta);

            // handled hooker activated if need
            invoke_meta_handled_hooker(node_name_, out_queue_.size(), out_meta);

            // notify consumer of out_queue
            this->out_queue_semaphore_.signal();
            TX_DEBUG(tx_utils::string_format("[%s] after handling meta, out_queue.size()==>%d", node_name_.c_str(), out_queue_.size()));
        }

        // batch by batch mode
        if (batch_complete && node_type() != NodeType::DES) {
            // push to out_queue one by one
            for (auto &i : frame_meta_batch_cache) {
                TX_DEBUG(tx_utils::string_format("[%s] before handling meta, out_queue.size()==>%d", node_name_.c_str(), out_queue_.size()));
                this->out_queue_.push(i);

                // handled hooker activated if need
                invoke_meta_handled_hooker(node_name_, out_queue_.size(), i);

                // notify consumer of out_queue
                this->out_queue_semaphore_.signal();
                TX_DEBUG(tx_utils::string_format("[%s] after handling meta, out_queue.size()==>%d", node_name_.c_str(), out_queue_.size()));
            }
            // clean cache for the next batch
            frame_meta_batch_cache.clear();
        }
    }
    // send dead flag for dispatch_thread
    this->out_queue_.push(nullptr);
    this->out_queue_semaphore_.signal();
}

// there is only one thread poping from the out_queue, we don't use lock here when poping.
void Node::dispatch_run() {
    while (alive_) {
        // wait for producer, make sure out_queue is not empty.
        this->out_queue_semaphore_.wait();

        TX_DEBUG(tx_utils::string_format("[%s] before dispatching meta, out_queue.size()==>%d", node_name_.c_str(), out_queue_.size()));
        auto out_meta = this->out_queue_.front();
        // dead flag
        if (out_meta == nullptr) {
            continue;
        }

        // leaving hooker activated if need
        invoke_meta_leaving_hooker(node_name_, out_queue_.size(), out_meta);

        // do something..
        this->push_meta(out_meta);
        this->out_queue_.pop();
        TX_DEBUG(tx_utils::string_format("[%s] after dispatching meta, out_queue.size()==>%d", node_name_.c_str(), out_queue_.size()));
    }
}

std::shared_ptr<tx_objects::Meta> Node::handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta) {
    return meta;
}

std::shared_ptr<tx_objects::Meta> Node::handle_control_meta(std::shared_ptr<tx_objects::ControlMeta> meta) {
    return meta;
}

void Node::handle_frame_meta(const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &meta_with_batch) {
}

void Node::meta_flow(std::shared_ptr<tx_objects::Meta> meta) {
    if (meta == nullptr) {
        return;
    }

    std::lock_guard<std::mutex> guard(this->in_queue_lock_);
    TX_DEBUG(tx_utils::string_format("[%s] before meta flow, in_queue.size()==>%d", node_name_.c_str(), in_queue_.size()));
    this->in_queue_.push(meta);

    // arriving hooker activated if need
    invoke_meta_arriving_hooker(node_name_, in_queue_.size(), meta);

    // notify consumer of in_queue
    this->in_queue_semaphore_.signal();
    TX_DEBUG(tx_utils::string_format("[%s] after meta flow, in_queue.size()==>%d", node_name_.c_str(), in_queue_.size()));
}

void Node::detach() {
    for (auto i : this->pre_nodes_) {
        i->remove_subscriber(shared_from_this());
    }
    this->pre_nodes_.clear();
}

void Node::detach_from(std::vector<std::string> pre_node_names) {
    for (auto i = this->pre_nodes_.begin(); i != this->pre_nodes_.end();) {
        if (std::find(pre_node_names.begin(), pre_node_names.end(), (*i)->node_name_) != pre_node_names.end()) {
            (*i)->remove_subscriber(shared_from_this());
            i = this->pre_nodes_.erase(i);
        } else {
            i++;
        }
    }
}

void Node::detach_recursively() {
    detach();
    auto nodes = next_nodes();
    for (auto &n : nodes) {
        n->detach_recursively();
    }
}

void Node::attach_to(std::vector<std::shared_ptr<Node>> pre_nodes) {
    // can not attach src node to any previous nodes
    if (this->node_type() == NodeType::SRC) {
        throw tx_excepts::InvalidCallingError("SRC nodes must not have any previous nodes!");
    }
    // can not attach any nodes to des node
    for (auto i : pre_nodes) {
        if (i->node_type() == NodeType::DES) {
            throw tx_excepts::InvalidCallingError("DES nodes must not have any next nodes!");
        }
        i->add_subscriber(shared_from_this());
        this->pre_nodes_.push_back(i);
    }
}

void Node::initialized() {
    // start threads since all resources have been initialized
    this->handle_thread_ = std::thread(&Node::handle_run, this);
    this->dispatch_thread_ = std::thread(&Node::dispatch_run, this);
}

void Node::deinitialized() {
    // send dead flag
    alive_ = false;
    {
        std::lock_guard<std::mutex> guard(this->in_queue_lock_);
        this->in_queue_.push(nullptr);
        this->in_queue_semaphore_.signal();
    }
    // wait for threads exits in vp_node
    if (handle_thread_.joinable()) {
        handle_thread_.join();
    }
    if (dispatch_thread_.joinable()) {
        dispatch_thread_.join();
    }
}

NodeType Node::node_type() {
    // return vp_node_type::MID by default
    // need override in child class
    return NodeType::MID;
}

std::vector<std::shared_ptr<Node>> Node::next_nodes() {
    std::vector<std::shared_ptr<Node>> next_nodes;
    std::lock_guard<std::mutex> guard(this->subscribers_lock_);
    for (auto &i : this->subscribers_) {
        next_nodes.push_back(std::dynamic_pointer_cast<Node>(i));
    }
    return next_nodes;
}

std::string Node::to_string() {
    // return node_name by default
    return node_name_;
}

void Node::pendding_meta(std::shared_ptr<tx_objects::Meta> meta) {
    this->out_queue_.push(meta);
    // handled hooker activated if need
    invoke_meta_handled_hooker(node_name_, out_queue_.size(), meta);
    // notify consumer of out_queue
    this->out_queue_semaphore_.signal();
}
} // namespace tx_nodes
