//
// Created by Yuting Xie on 03/04/2023.
//

#include "TaskGraph.h"

#include <cstdio>
#include <stack>

namespace gogort {

bool TaskGraph::AddTask(std::string task_name, std::vector<std::string> pipe_in,
                        std::vector<std::string> pipe_out) {
  assert(name_to_node_pair_.contains(task_name) == false);

  // Task state nodes
  auto ready_node = std::make_shared<Node>(task_name);
  auto finish_node = std::make_shared<Node>(task_name);
  ready_node->type_ = StateNode::kNodeReady;
  finish_node->type_ = StateNode::kNodeFinish;
  name_to_node_pair_.insert({task_name, {ready_node, finish_node}});
  if (pipe_in.empty()) {
    sensor_nodes_.push_back(ready_node);
  }

  // Task edge
  auto task_edge = std::make_shared<Edge>(ready_node, finish_node);
  task_edge->type_ = DirectedEdge::kEdgeTask;
  ready_node->edge_out_.push_back(task_edge);

  // Examine dependencies
  for (auto &pipe : pipe_in) {
    // Must first finish the sync process then add new dependencies.
    auto &[vec_pub, vec_sub] = pipe_to_task_[pipe];
    for (auto &task_pub : vec_pub) {
      assert(name_to_node_pair_.contains(task_pub));
      auto finish_pubber = name_to_node_pair_[task_pub][1];
      // Add the sync edge for <pubber, subber>
      auto sync_edge = std::make_shared<Edge>(finish_pubber, ready_node);
      sync_edge->type_ = DirectedEdge::kEdgeSync;
      finish_pubber->edge_out_.push_back(sync_edge);
    }
    vec_sub.push_back(task_name);
  }
  for (auto &pipe : pipe_out) {
    // Must first finish the sync process then add new dependencies.
    auto &[vec_pub, vec_sub] = pipe_to_task_[pipe];
    for (auto &task_sub : vec_sub) {
      assert(name_to_node_pair_.contains(task_sub));
      auto ready_subber = name_to_node_pair_[task_sub][0];
      // Add the sync edge for <pubber, subber>
      auto sync_edge = std::make_shared<Edge>(finish_node, ready_subber);
      sync_edge->type_ = DirectedEdge::kEdgeSync;
      finish_node->edge_out_.push_back(sync_edge);
    }
    vec_pub.push_back(task_name);
  }

  // Cong! The graph back to connected component now!

  return true;
}

void TaskGraph::Display() {
  // All tasks
  for (auto &[task_name, nodes] : name_to_node_pair_) {
    printf("Task name: %s\n", task_name.c_str());
  }

  // DFS from sensors
  std::unordered_map<uint64_t, bool> dfs_track;
  for (auto &sensor : sensor_nodes_) {
    std::stack<std::shared_ptr<Node>> stk;
    stk.push(sensor);
    while (!stk.empty()) {
      auto cur = stk.top();
      stk.pop();
      printf("%s_%s -> ", cur->task_name_.c_str(),
             cur->type_ == StateNode::kNodeReady ? "ready" : "finish");
      for (auto &nxt : cur->edge_out_) {
        if (!dfs_track[nxt->to_->id_]) {
          dfs_track[nxt->to_->id_] = true;
          stk.push(nxt->to_);
        }
      }
    }
    printf("\n");
  }
}

} // namespace gogort