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

#include <openscenario_interpreter/reader/element.hpp>
#include <openscenario_interpreter/syntax/deterministic_multi_parameter_distribution.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
DeterministicMultiParameterDistribution::DeterministicMultiParameterDistribution(
  const pugi::xml_node & node, Scope & scope)
: DeterministicMultiParameterDistributionType(node, scope)
{
}

auto DeterministicMultiParameterDistribution::derive() -> ParameterDistribution
{
  return DeterministicMultiParameterDistributionType::derive();
}

auto DeterministicMultiParameterDistribution::derive(
  std::size_t local_index, std::size_t local_size, std::size_t global_index,
  std::size_t global_size) -> ParameterList
{
  return DeterministicMultiParameterDistributionType::derive(
    local_index, local_size, global_index, global_size);
}

auto DeterministicMultiParameterDistribution::getNumberOfDeriveScenarios() const -> std::size_t
{
  return DeterministicMultiParameterDistributionType::getNumberOfDeriveScenarios();
}
}  // namespace syntax
}  // namespace openscenario_interpreter
