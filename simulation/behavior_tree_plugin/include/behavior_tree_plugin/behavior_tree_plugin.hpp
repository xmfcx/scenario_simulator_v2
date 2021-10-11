// Copyright 2015-2020 Tier IV, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BEHAVIOR_TREE_PLUGIN__BEHAVIOR_TREE_PLUGIN_HPP_
#define BEHAVIOR_TREE_PLUGIN__BEHAVIOR_TREE_PLUGIN_HPP_

#include <behaviortree_cpp_v3/bt_factory.h>
#include <behaviortree_cpp_v3/loggers/bt_cout_logger.h>

#include <behavior_tree_plugin/pedestrian/follow_lane_action.hpp>
#include <behavior_tree_plugin/pedestrian/walk_straight_action.hpp>
#include <functional>
#include <geometry_msgs/msg/point.hpp>
#include <map>
#include <memory>
#include <openscenario_msgs/msg/entity_status.hpp>
#include <string>
#include <traffic_simulator/behavior/behavior_plugin_base.hpp>
#include <traffic_simulator/hdmap_utils/hdmap_utils.hpp>
#include <vector>

namespace entity_behavior
{
class BehaviorTreePlugin : public BehaviorPluginBase
{
public:
  void configure();
  void update(double current_time, double step_time) override;
  const std::string getCurrentAction() const { return current_action_; }
  template <typename T>
  void setValueToBlackBoard(std::string key, T value)
  {
    tree_.rootBlackboard()->set(key, value);
  }
  template <typename T>
  void getValueToBlackBoard(std::string key, T & value)
  {
    tree_.rootBlackboard()->get(key, value);
  }

protected:
  BT::NodeStatus tickOnce(double current_time, double step_time);
  std::string request_;
  BT::BehaviorTreeFactory factory_;
  BT::Tree tree_;
  std::shared_ptr<BT::StdCoutLogger> logger_cout_ptr_;
  void callback(
    BT::Duration timestamp, const BT::TreeNode & node, BT::NodeStatus prev_status,
    BT::NodeStatus status);
  void setupLogger();
  BT::TimestampType type_;
  BT::TimePoint first_timestamp_;
  std::vector<BT::TreeNode::StatusChangeSubscriber> subscribers_;
  std::string current_action_;
};
}  // namespace entity_behavior

#endif  // BEHAVIOR_TREE_PLUGIN__BEHAVIOR_TREE_PLUGIN_HPP_
