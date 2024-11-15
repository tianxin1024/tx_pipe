#pragma once

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <memory>
#include <chrono>

#include "utils/semaphore.h"
#include "utils/utils.h"
#include "utils/logger.h"
#include "nodes/meta_publisher.h"
#include "nodes/meta_hookable.h"
#include "objects/control_meta.h"
#include "objects/frame_meta.h"
#include "utils/invalid_error.h"

namespace tx_nodes {
// node type
enum NodeType {
    SRC, // src node, must not have input branchs
    DES, // des node, must not have output branchs
    MID  // middle node, can have input and output branchs
};

// base class for all nodes
class Node : public MetaPublisher,
             public MetaSubscriber,
             public MetaHookable,
             public std::enable_shared_from_this<Node> {
private:
    // previous nodes
    std::vector<std::shared_ptr<Node>> pre_nodes_;

    // handle thread
    std::thread handle_thread_;
    // dispatch thread
    std::thread dispatch_thread_;

protected:
    // alive or not for node
    bool alive_ = true;

    // by default we handle frame meta one by one, in some situations we need handle them batch by batch(such as vp_infer_node).
    // setting this member greater than 1 means the node will handle frame meta with batch, and vp_node::handle_frame_meta_by_batch(...) will be called other than vp_node::handle_frame_meta(...).
    // note: control meta is not allowed like above, only one by one supported.
    int frame_meta_handle_batch_ = 1;

    // cache input meta from previous nodes
    std::queue<std::shared_ptr<tx_objects::Meta>> in_queue_;
    //
    std::mutex in_queue_lock_;
    // cache output meta to next nodes
    std::queue<std::shared_ptr<tx_objects::Meta>> out_queue_;

    // synchronize for in_queue
    tx_utils::Semaphore in_queue_semaphore_;
    // synchronize for out_queue
    tx_utils::Semaphore out_queue_semaphore_;

    // get meta from in_queue, handle meta and put them into out_queue looply.
    // we need re-implement(define how to create meta and put it into out_queue) in src nodes since they have no previous nodes.
    virtual void handle_run();
    // get meta from out_queue and push them to next nodes looply.
    // we need re-implement(just do nothing) in des nodes since they have no next nodes.
    virtual void dispatch_run();

    // define how to handle frame meta [one by one], ignored in src nodes.
    // return nullptr means do not push it to next nodes, such as des nodes which have no next nodes.
    virtual std::shared_ptr<tx_objects::Meta> handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta);
    // define how to handle control meta, ignored in src nodes.
    // return nullptr means do not push it to next nodes, such as des nodes which have no next nodes.
    virtual std::shared_ptr<tx_objects::Meta> handle_control_meta(std::shared_ptr<tx_objects::ControlMeta> meta);

    // define how to handle frame meta [batch by batch], ignored in src nodes.
    virtual void handle_frame_meta(const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &meta_with_batch);

    // called by child classes after all resources have been initialized (in the last constructor of chain).
    void initialized();
    // called by child classes before all resources going to be destroyed (in the last destructor of chain)。
    virtual void deinitialized();

    // push meta to the back of out_queue, then it will be pushed to next nodes in order.
    // take care it's different from vp_node::push_meta(meta) which will push meta to next nodes directly.
    // the method can be called ONLY in handle thread inside node.
    void pendding_meta(std::shared_ptr<tx_objects::Meta> meta);

    // protected as it can't be instanstiated directly.
    Node(std::string node_name);

public:
    virtual ~Node();
    // clear meaningful string, such as 'file_src_0' stands for file source node at channel 0.
    std::string node_name_;

    // receive meta from previous nodes,
    // we can hook(such as modifying meta) in child class but do not forget calling vp_node.meta_flow(...) followly.
    virtual void meta_flow(std::shared_ptr<tx_objects::Meta> meta) override;

    // retrive current node type
    virtual NodeType node_type();

    // detach myself from all previous nodes
    void detach();
    // detach myself from specific previous nodes
    void detach_from(std::vector<std::string> pre_node_names);
    // detach myself from all previous nodes AND the same action on all next nodes(recursively), can be used to split the whole pipeline into single nodes before process exits.
    void detach_recursively();
    // attach myself to previous nodes(can be a list)
    void attach_to(std::vector<std::shared_ptr<Node>> pre_nodes);

    // get next nodes
    std::vector<std::shared_ptr<Node>> next_nodes();

    // get description of node
    virtual std::string to_string();
};

} // namespace tx_nodes
