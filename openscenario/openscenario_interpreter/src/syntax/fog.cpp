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
#include <openscenario_interpreter/syntax/bounding_box.hpp>
#include <openscenario_interpreter/syntax/double.hpp>
#include <openscenario_interpreter/syntax/fog.hpp>

namespace openscenario_interpreter
{
inline namespace syntax
{
Fog::Fog(const pugi::xml_node & node, Scope & scope)
: visual_range(readAttribute<Double>("visualRange", node, scope)),
  bounding_box(readElement<BoundingBox>("BoundingBox", node, scope))
{
  // Valid range information:
  // https://www.asam.net/static_downloads/ASAM_OpenSCENARIO_V1.2.0_Model_Documentation/modelDocumentation/content/Fog.html

  if (auto visual_range_valid = 0 <= visual_range; not visual_range_valid) {
    THROW_SYNTAX_ERROR(std::quoted("Fog::visualRange"), "is out of range [0..inf[");
  }
}
}  // namespace syntax
}  // namespace openscenario_interpreter
