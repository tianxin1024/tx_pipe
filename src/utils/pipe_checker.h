#pragma once

#include <sstream>

#include "nodes/node.h"
#include "utils/invalid_error.h"

namespace tx_utils {

class PipeChecker {
private:
    // we can only handle the pipeline structure like below, totally symmetrical vertically for each layer.
    /*
        * |1st layer|        |2nd layer|      |3rd layer|     |4th layer|       |5th layer|
        *    node     -...-     node \                         / node     -...-    node
        * 
        *    node     -...-     node -   -...-   node    -...-   node     -...-    node
        * 
        *    node     -...-     node /                         \ node     -...-    node
        *                                                                                           
        */
    void check_layer(std::vector<std::shared_ptr<tx_nodes::Node>> nodes_in_layer) {
        num_layers_in_pipe++;

        int next_nodes_num = -1;
        std::vector<std::shared_ptr<tx_nodes::Node>> all_next_nodes;

        // check for:
        // 1. node type of start/end point of pipe
        // 2. node names in pipe
        // 3. next nodes number
        for (auto &i : nodes_in_layer) {
            // check for the first layer, all nodes MUST be tx_nodes::Node_type::SRC
            if (num_layers_in_pipe == 1 && i->node_type() != tx_nodes::NodeType::SRC) {
                throw tx_excepts::InvalidPipelineError("pipe MUST starts with SRC nodes!");
            }

            auto name = i->node_name_;
            // check if node name duplicated
            if (std::find_if(std::begin(node_names_in_pipe),
                             std::end(node_names_in_pipe),
                             [&](std::pair<int, std::vector<std::string>> p) { return std::find(std::begin(p.second), std::end(p.second), name) != std::end(p.second); })
                != std::end(node_names_in_pipe)) {
                throw tx_excepts::InvalidPipelineError("node names MUST not be duplicated in pipe!");
            }
            node_names_in_pipe[num_layers_in_pipe].push_back(name);

            auto next_nodes = i->next_nodes();
            auto _num = next_nodes.size();

            // check for the last layer, all nodes MUST be tx_nodes::Node_type::DES
            if (_num == 0 && i->node_type() != tx_nodes::NodeType::DES) {
                throw tx_excepts::InvalidPipelineError("pipe MUST ends with DES nodes!");
            }

            // initialize next_nodes_num variable, all nodes in current layer should have the same next_nodes_num.
            if (next_nodes_num == -1) {
                next_nodes_num = _num;
            }

            // the next nodes number of each node in the same layer must be equal (all are 0 for des nodes).
            if (next_nodes_num != _num) {
                throw tx_excepts::InvalidPipelineError("all nodes in the same layer MUST have the same next nodes number!");
            }

            // merge next nodes of each node into a big vector
            all_next_nodes.insert(all_next_nodes.end(), next_nodes.begin(), next_nodes.end());
        }

        // check for 2 conditions:
        // 1. all next nodes are the same, or
        // 2. all next nodes are distinc.
        if (all_next_nodes.size() != 0) {
            bool all_the_same = true;
            bool all_distinct = true;
            // check if all the next nodes are the same
            for (auto &i : all_next_nodes) {
                if (i != all_next_nodes[0]) {
                    all_the_same = false;
                    break;
                }
            }

            // check if all next nodes are distinct
            if (!all_the_same) {
                std::sort(std::begin(all_next_nodes), std::end(all_next_nodes));
                auto pos = std::adjacent_find(std::begin(all_next_nodes), std::end(all_next_nodes));
                if (pos != std::end(all_next_nodes)) {
                    all_distinct = false;
                }
            }

            if (!all_the_same && !all_distinct) {
                throw tx_excepts::InvalidPipelineError("the next nodes of each node in the same layer MUST be the same or be distinct totally!");
            }

            // just keep the first one if all the next nodes are the same
            if (all_the_same) {
                all_next_nodes.erase(all_next_nodes.begin() + 1, all_next_nodes.end());
            }

            // recursion for next layer
            check_layer(all_next_nodes);
        }
    }
    // layer_index -> node_names
    std::map<int, std::vector<std::string>> node_names_in_pipe;
    int num_layers_in_pipe = 0;
    bool summary;

public:
    PipeChecker(bool summary = true) {
        summary = summary;
    }

    // total layers
    int pipe_width() {
        return num_layers_in_pipe;
    }

    // max number of nodes in all layers
    int pipe_height() {
        int number = 0;
        for (auto &i : node_names_in_pipe) {
            if (i.second.size() > number) {
                number = i.second.size();
            }
        }
        return number;
    }

    void operator()(std::vector<std::shared_ptr<tx_nodes::Node>> pipe_src_nodes) {
        // start check with the 1st layer
        check_layer(pipe_src_nodes);

        if (summary) {
            /*
                std::cout << "############# pipe check summary ##############" << std::endl;
                std::cout << " total layers: " << num_layers_in_pipe << std::endl;
                std::cout << " layer index,       node names" << std::endl;
                for(auto & i : node_names_in_pipe) {
                    std::cout << " " << i.first << "                    ";
                    for(auto & j : i.second) {
                        std::cout << j << ",";
                    }
                    std::cout << std::endl;
                }
                std::cout << ############# pipe check summary ############## << std::endl;
                */

            std::ostringstream s_stream;
            s_stream << "\n############# pipe check summary ##############\n";
            s_stream << " total layers: " << num_layers_in_pipe << "\n";
            s_stream << " layer index,       node names"
                     << "\n";
            for (auto &i : node_names_in_pipe) {
                s_stream << " " << i.first << "                    ";
                for (auto &j : i.second) {
                    s_stream << j << ",";
                }
                s_stream << "\n";
            }
            s_stream << "############# pipe check summary ##############"
                     << "\n";

            // to log
            TX_INFO(s_stream.str());
        }
    }
};
} // namespace tx_utils
