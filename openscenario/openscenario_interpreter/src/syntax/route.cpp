// Copyright 2015 TIER IV, Inc. All rights reserved.
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

#include <openscenario_interpreter/reader/attribute.hpp>
#include <openscenario_interpreter/reader/element.hpp>
#include <openscenario_interpreter/syntax/route.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
Route::Route(const pugi::xml_node & node, Scope & scope)
: Scope(readAttribute<String>("name", node, scope), scope),
  closed(readAttribute<Boolean>("closed", node, local(), Boolean())),
  parameter_declarations(readElement<ParameterDeclarations>("ParameterDeclarations", node, local()))
{
  traverse<2, unbounded>(
    node, "Waypoint", [&](auto && node) { return waypoints.emplace_back(node, local()); });
}

Route::operator std::vector<traffic_simulator::LaneletPoseType>() const
{
  std::vector<traffic_simulator::LaneletPoseType> lanelet_poses{};

  for (const auto & waypoint : waypoints) {
    lanelet_poses.emplace_back(static_cast<traffic_simulator::LaneletPoseType>(waypoint));
  }

  return lanelet_poses;
}

Route::operator std::vector<traffic_simulator::CanonicalizedLaneletPoseType>() const
{
  std::vector<traffic_simulator::CanonicalizedLaneletPoseType> lanelet_poses{};

  for (const auto & waypoint : waypoints) {
    lanelet_poses.emplace_back(
      static_cast<traffic_simulator::CanonicalizedLaneletPoseType>(waypoint));
  }

  return lanelet_poses;
}
}  // namespace syntax
}  // namespace openscenario_interpreter
