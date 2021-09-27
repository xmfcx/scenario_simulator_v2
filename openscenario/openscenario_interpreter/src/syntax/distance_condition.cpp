// Copyright 2015-2021 Tier IV, Inc. All rights reserved.
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

#include <cmath>
#include <openscenario_interpreter/procedure.hpp>
#include <openscenario_interpreter/syntax/distance_condition.hpp>
#include <sstream>

namespace openscenario_interpreter
{
inline namespace syntax
{
auto DistanceCondition::description() const -> std::string
{
  std::stringstream description;

  description << triggering_entities.description() << "'s distance to given position = ";

  print_to(description, last_checked_values);

  description << " " << rule << " " << value << "?";

  return description.str();
}

auto DistanceCondition::evaluate() -> Element
{
  auto distance = [&](auto && name) {
    const auto pose = getRelativePose(name, static_cast<geometry_msgs::msg::Pose>(position));
    return std::hypot(pose.position.x, pose.position.y);
  };

  last_checked_values.clear();

  return asBoolean(triggering_entities.apply([&](auto && triggering_entity) {
    last_checked_values.push_back(distance(triggering_entity));
    return rule(last_checked_values.back(), value);
  }));
}
}  // namespace syntax
}  // namespace openscenario_interpreter
