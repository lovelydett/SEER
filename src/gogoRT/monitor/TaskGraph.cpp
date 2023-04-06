//
// Created by Yuting Xie on 03/04/2023.
//

#include "TaskGraph.h"
#include "../utils/time_utils.h"

#include <cassert>
#include <cstdio>
#include <stack>

namespace gogort {

std::mutex TaskGraph::mtx_;

bool TaskGraph::AddTask(std::string task_name, std::vector<std::string> pipe_in,
                        std::vector<std::string> pipe_out) {
  assert(name_to_node_pair_.contains(task_name) == false);

  if (pipe_out.empty()) {
    assert(exit_task_name_.empty() && "Redundant exit task!");
    exit_task_name_ = task_name;
  }

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

void TaskGraph::OnTaskUpdate(const std::string &task_name,
                             const UpdateType type) {
  // Temp fix
  if (task_name.find("risk") != std::string::npos) {
    return;
  }
  std::lock_guard<std::mutex> lock(mtx_);
  // First validate the task status
  auto &[ready_node, finish_node] = name_to_node_pair_[task_name];
  ready_node->timestamp_ms_ = get_current_timestamp_ms();
  finish_node->timestamp_ms_ = get_current_timestamp_ms();
  switch (type) {
  case kUpAssign: {
    ready_node->status_ = StateNode::kStatusAssigned;
    finish_node->status_ = StateNode::kStatusAssigned;
    ready_node->round_++;
    break;
  }
  case kUpPreempt: {
    ready_node->status_ = StateNode::kStatusPreempting;
    finish_node->status_ = StateNode::kStatusPreempting;
    ready_node->round_++;
    break;
  }
  case kUpExecute: {
    ready_node->status_ = StateNode::kStatusRunning;
    finish_node->status_ = StateNode::kStatusRunning;
    break;
  }
  case kUpFinish: {
    ready_node->status_ = StateNode::kStatusNone;
    finish_node->status_ = StateNode::kStatusNone;
    finish_node->round_++; // Set finish_node->round_ here
    assert(finish_node->round_ == ready_node->round_ &&
           "Finish round should be equal to ready round now!");
    break;
  }
  default:
    assert(false && "Invalid update type!");
  }
}

// Display the realtime graph trace data
void TaskGraph::DisplayDynamic() {
  assert(!name_to_node_pair_.empty());
  printf("Displaying dynamic graph trace data:\n");
  static uint64_t time_base_ms = get_current_timestamp_ms();
  // Only run once to get the dfs order
  static const auto dfs_order = [&]() -> std::vector<std::string> {
    std::vector<std::string> dfs_order;
    std::unordered_map<uint64_t, bool> dfs_track;
    for (auto &sensor : sensor_nodes_) {
      std::stack<std::shared_ptr<Node>> stk;
      stk.push(sensor);
      while (!stk.empty()) {
        auto cur = stk.top();
        stk.pop();
        if (dfs_track[cur->id_]) {
          continue;
        }
        dfs_track[cur->id_] = true;
        dfs_order.push_back(cur->task_name_);
        for (auto &nxt : cur->edge_out_) {
          stk.push(nxt->to_);
        }
      }
    }
    return dfs_order;
  }();

  for (auto &task_name : dfs_order) {
    auto &[ready_node, finish_node] = name_to_node_pair_[task_name];
    printf("Task: %s,\tround: %lu,\tstatus: %d,\tts: %lu\n", task_name.c_str(),
           finish_node->round_, finish_node->status_,
           finish_node->timestamp_ms_);
  }
}

void TaskGraph::DisplayStatic() {
  // All tasks
  for (auto &[task_name, nodes] : name_to_node_pair_) {
    printf("Task name: %s\n", task_name.c_str());
    assert(nodes.size() == 2);
    auto &[ready_node, finish_node] = nodes;
    assert(ready_node);
    assert(finish_node);
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