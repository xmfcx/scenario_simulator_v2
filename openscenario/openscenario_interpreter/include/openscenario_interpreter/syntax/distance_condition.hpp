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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__DISTANCE_CONDITION_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__DISTANCE_CONDITION_HPP_

#include <openscenario_interpreter/syntax/boolean.hpp>
#include <openscenario_interpreter/syntax/coordinate_system.hpp>
#include <openscenario_interpreter/syntax/double.hpp>
#include <openscenario_interpreter/syntax/position.hpp>
#include <openscenario_interpreter/syntax/relative_distance_type.hpp>
#include <openscenario_interpreter/syntax/rule.hpp>
#include <openscenario_interpreter/syntax/triggering_entities.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- DistanceCondition (OpenSCENARIO 1.1) -----------------------------------
 *
 *  The current distance between an entity and a position is compared to a
 *  given distance (less, greater, equal). Several additional parameters like
 *  free space etc. can be defined. The property "alongRoute" is deprecated. If
 *  "coordinateSystem" or "relativeDistanceType" are set, "alongRoute" is
 *  ignored.
 *
 *  <xsd:complexType name="DistanceCondition">
 *    <xsd:all>
 *      <xsd:element name="Position" type="Position"/>
 *    </xsd:all>
 *    <xsd:attribute name="alongRoute" type="Boolean">
 *      <xsd:annotation>
 *        <xsd:appinfo>
 *          deprecated
 *        </xsd:appinfo>
 *      </xsd:annotation>
 *    </xsd:attribute>
 *    <xsd:attribute name="coordinateSystem" type="CoordinateSystem"/>
 *    <xsd:attribute name="freespace" type="Boolean" use="required"/>
 *    <xsd:attribute name="relativeDistanceType" type="RelativeDistanceType"/>
 *    <xsd:attribute name="rule" type="Rule" use="required"/>
 *    <xsd:attribute name="value" type="Double" use="required"/>
 *  </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct DistanceCondition
{
  // Definition of the coordinate system to be used for calculations. If not provided the value is interpreted as "entity". If set, "alongRoute" is ignored.
  const CoordinateSystem coordinate_system;

  // True: distance is measured between closest bounding box points. False: reference point distance is used.
  const Boolean freespace;

  // Definition of the coordinate system dimension(s) to be used for calculating distances. If set, "alongRoute" is ignored. If not provided, value is interpreted as "euclideanDistance".
  const RelativeDistanceType relative_distance_type;

  // The operator (less, greater, equal).
  const Rule rule;

  // The distance value. Unit: m; Range: [0..inf[.
  const Double value;

  // The given position the distance is related to.
  const Position position;

  const TriggeringEntities triggering_entities;

  std::vector<Double> last_checked_values;  // for description

  template <typename Node, typename Scope>
  explicit DistanceCondition(
    const Node & node, Scope & scope, const TriggeringEntities & triggering_entities)
  // clang-format off
  : coordinate_system     (readAttribute<CoordinateSystem    >("coordinateSystem",     node, scope, CoordinateSystem::entity)),
    freespace             (readAttribute<Boolean             >("freespace",            node, scope)),
    relative_distance_type(readAttribute<RelativeDistanceType>("relativeDistanceType", node, scope)),
    rule                  (readAttribute<Rule                >("rule",                 node, scope)),
    value                 (readAttribute<Double              >("value",                node, scope)),
    position              (readElement  <Position            >("Position",             node, scope)),
    triggering_entities(triggering_entities),
    last_checked_values(triggering_entities.entity_refs.size(), Double::nan())
  // clang-format on
  {
  }

  auto description() const -> std::string;

  auto evaluate() -> Element;
};
}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__DISTANCE_CONDITION_HPP_
