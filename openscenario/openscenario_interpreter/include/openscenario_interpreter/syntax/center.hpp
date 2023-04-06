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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__CENTER_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__CENTER_HPP_

#ifndef WITHOUT_ROS
#include <geometry_msgs/msg/point.hpp>
#else
namespace geometry_msgs::msg
{
struct Point
{
  double x, y, z;
};
}  // namespace geometry_msgs::msg
#endif  // WITHOUT_ROS

#include <openscenario_interpreter/scope.hpp>
#include <pugixml.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- Center -----------------------------------------------------------------
 *
 *  <xsd:complexType name="Center">
 *    <xsd:attribute name="x" type="Double" use="required"/>
 *    <xsd:attribute name="y" type="Double" use="required"/>
 *    <xsd:attribute name="z" type="Double" use="required"/>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct Center
{
  const Double x, y, z;

  Center() = default;

  explicit Center(const pugi::xml_node &, Scope &);

  explicit operator geometry_msgs::msg::Point() const;
};
}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__CENTER_HPP_
