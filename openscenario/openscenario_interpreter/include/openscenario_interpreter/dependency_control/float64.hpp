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

#ifndef OPENSCENARIO_PREPROCESSOR_FLOAT64_HPP_
#define OPENSCENARIO_PREPROCESSOR_FLOAT64_HPP_

#ifndef WITHOUT_ROS
#include <std_msgs/msg/float64.hpp>
#else
namespace std_msgs::msg
{
struct Float64
{
  using value_type = double;

  value_type data;

  explicit Float64(value_type value = 0.0) : data(value){};
};
}  // namespace std_msgs::msg
#endif

#endif  //OPENSCENARIO_PREPROCESSOR_FLOAT64_HPP
