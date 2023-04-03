// The state-on-vertex (SOV) abstraction of an event-driven system.
// Created by Yuting Xie on 03/04/2023.
//

#ifndef GOGO_PROJ_TASKGRAPH_H
#define GOGO_PROJ_TASKGRAPH_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace gogort {

struct StateNode;

typedef struct DirectedEdge {
  enum EdgeType { kEdgeTask = 1, kEdgeSync };
  EdgeType type_;
  // std::string name_; // task name for kEdgeTask, paired task names for
  // kEdgeSync const uint64_t id_;
  std::string from_name_;
  std::string to_name_;
  uint64_t length_ms_; // Track the last duration in ms
  std::shared_ptr<struct StateNode> v_in_;
  std::shared_ptr<struct StateNode> v_out_;
} Edge;

typedef struct StateNode {
  enum NodeType { kNodeReady = 1, kNodeFinish };
  NodeType type_;
  std::string name_;
  const uint64_t id_;     // Do we need paired id?
  uint64_t timestamp_ms_; // Track the last triggered moment
  int count_;             // VERY critical, track the task graph round
  std::vector<std::shared_ptr<Edge>> edge_out_;
} Node;

enum DependencyType { kPub = 1, kSub };
enum UpdateType { kInvoke = 1, kAssign, kExecute, kFinish };

class TaskGraph {
private:
  std::unordered_map<uint64_t, std::shared_ptr<Node>> id_to_node_;
  std::vector<std::shared_ptr<Node>> nodes_;

public:
  TaskGraph() = default;
  [[nodiscard]] bool AddTask(std::string task_name);
  [[nodiscard]] bool AddDependency(std::string task_name, std::string pipe_name,
                                   DependencyType type);
  void OnTaskUpdate(std::string task_name, UpdateType type);
};

} // namespace gogort

#endif // GOGO_PROJ_TASKGRAPH_H
