// The state-on-vertex (SOV) abstraction of an event-driven system.
// Created by Yuting Xie on 03/04/2023.
//

#ifndef GOGO_PROJ_TASKGRAPH_H
#define GOGO_PROJ_TASKGRAPH_H

#include "../utils/utils.h"

#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace gogort {

struct StateNode;

typedef struct DirectedEdge {
  enum EdgeType { kEdgeNone = 0, kEdgeTask, kEdgeSync };
  EdgeType type_ = kEdgeNone;
  uint64_t length_ms_ = 0; // Track the last duration in ms
  const std::shared_ptr<struct StateNode> from_;
  const std::shared_ptr<struct StateNode> to_;
  DirectedEdge(std::shared_ptr<struct StateNode> from,
               std::shared_ptr<struct StateNode> to)
      : from_(from), to_(to) {}
} Edge;

typedef struct StateNode {
  enum NodeType { kNodeNone = 0, kNodeReady, kNodeFinish };
  NodeType type_ = kNodeNone;
  const std::string task_name_;
  const uint64_t id_;
  uint64_t timestamp_ms_ = 0; // Track the last triggered moment
  int count_ = 0;             // VERY critical, track the task graph round
  std::vector<std::shared_ptr<Edge>> edge_out_;
  explicit StateNode(std::string &task_name)
      : id_(get_next_uuid()), task_name_(task_name) {}
} Node;

enum DependencyType { kPub = 1, kSub };
enum UpdateType { kInvoke = 1, kAssign, kExecute, kFinish };

class TaskGraph {
private:
  std::unordered_map<std::string, std::array<std::shared_ptr<Node>, 2>>
      name_to_node_pair_;
  std::vector<std::shared_ptr<Node>>
      sensor_nodes_; // Only store sensor ready nodes
  std::unordered_map<std::string, std::array<std::vector<std::string>, 2>>
      pipe_to_task_;

public:
  TaskGraph() = default;
  [[nodiscard]] bool AddTask(std::string task_name,
                             std::vector<std::string> pipe_in,
                             std::vector<std::string> pipe_out);
  void Display();
  void OnTaskUpdate(std::string task_name, UpdateType type);
};

} // namespace gogort

#endif // GOGO_PROJ_TASKGRAPH_H
