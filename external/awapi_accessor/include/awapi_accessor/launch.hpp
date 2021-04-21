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

#ifndef AWAPI_ACCESSOR__LAUNCH_HPP_
#define AWAPI_ACCESSOR__LAUNCH_HPP_

#if AWAPI_CONCEALER_ISOLATE_STANDARD_OUTPUT
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif  // AWAPI_CONCEALER_ISOLATE_STANDARD_OUTPUT

#include <awapi_accessor/execute.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <system_error>
#include <type_traits>
#include <vector>

namespace awapi
{
template <typename... Ts>
static auto ros2_launch(const std::string & package, const std::string & file, Ts &&... xs)
{
#ifdef AUTOWARE_CONCEALER_ISOLATE_STANDARD_OUTPUT
  const std::string log_filename = "/tmp/scenario_test_runner/autoware-output.txt";
  const auto fd = ::open(log_filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  ::dup2(fd, STDOUT_FILENO);
  ::dup2(fd, STDERR_FILENO);
  ::close(fd);
#endif

  const auto process_id = fork();

  const std::vector<std::string> argv{
    "python3",
    "/opt/ros/foxy/bin/ros2",
    "launch",  // NOTE: The command 'ros2' is a Python script.
    package,
    file,
    std::forward<decltype(xs)>(xs)...};

  if (process_id < 0) {
    throw std::system_error(errno, std::system_category());
  } else if (process_id == 0 and execute(argv) < 0) {
    std::cout << std::system_error(errno, std::system_category()).what() << std::endl;
    std::exit(EXIT_FAILURE);
  } else {
    return process_id;
  }
}
}  // namespace awapi

#endif  // AWAPI_ACCESSOR__LAUNCH_HPP_
