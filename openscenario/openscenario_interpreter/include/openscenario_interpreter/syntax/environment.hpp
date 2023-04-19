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

#ifndef OPENSCENARIO_INTERPRETER__SYNTAX__ENVIRONMENT_HPP_
#define OPENSCENARIO_INTERPRETER__SYNTAX__ENVIRONMENT_HPP_

#include <openscenario_interpreter/scope.hpp>
#include <openscenario_interpreter/syntax/parameter_declarations.hpp>
#include <openscenario_interpreter/syntax/road_condition.hpp>
#include <openscenario_interpreter/syntax/time_of_day.hpp>
#include <openscenario_interpreter/syntax/weather.hpp>
#include <pugixml.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
/* ---- Environment 1.2 ----------------------------------------------------
 *
 * <xsd:complexType name="Environment">
 *   <xsd:all>
 *     <xsd:element name="ParameterDeclarations" type="ParameterDeclarations" minOccurs="0"/>
 *     <xsd:element name="TimeOfDay" type="TimeOfDay" minOccurs="0"/>
 *     <xsd:element name="Weather" type="Weather" minOccurs="0"/>
 *     <xsd:element name="RoadCondition" type="RoadCondition" minOccurs="0"/>
 *   </xsd:all>
 *   <xsd:attribute name="name" type="String" use="required"/>
 * </xsd:complexType>
 *
 * -------------------------------------------------------------------------- */
struct Environment : public Scope
{
  /*
   * Warn: Model specification(ref) has following statement.
   * > If one of the conditions is missing it means that it doesn't change.
   * However, current implementation does not represent that condition is missing.
   * Instead, conditions are filled with its default state (from default constructor).
   * This difference may cause unspecified behavior.
   * TODO: Follow the model specification as described above.
   *
   * ref:
   * https://www.asam.net/static_downloads/ASAM_OpenSCENARIO_V1.2.0_Model_Documentation/modelDocumentation/content/Environment.html
   */

  Environment() = default;

  explicit Environment(const pugi::xml_node &, Scope &);

  const ParameterDeclarations parameter_declarations;

  const TimeOfDay time_of_day;

  const Weather weather;

  const RoadCondition road_condition;
};

}  // namespace syntax
}  // namespace openscenario_interpreter

#endif  // OPENSCENARIO_INTERPRETER__SYNTAX__ENVIRONMENT_HPP_
