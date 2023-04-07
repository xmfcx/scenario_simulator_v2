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

//#include <glog/logging.h>

#include <boost/program_options.hpp>
#include <cstdlib>
#include <memory>
#include <openscenario_preprocessor/openscenario_preprocessor.hpp>

int main(const int argc, char const * const * const argv)
{
//  google::InitGoogleLogging(argv[0]);
//  google::InstallFailureSignalHandler();

  using namespace boost::program_options;
  options_description global("preprocessor command for OpenSCENARIO 1.2");

  // clang-format off
  global.add_options()
    ("command", value<std::string>(),              "subcommand")
    ("subargs" , value<std::vector<std::string>>(), "subcommand arguments");

  positional_options_description pos;
  pos.add("command", 1).add("subargs", -1);

  variables_map vm;

  parsed_options parsed = command_line_parser(argc, argv).
                          options(global).
                          positional(pos).
                          allow_unregistered().
                          run();

  // clang-format on

  store(parsed, vm);

  std::string sub_command = vm["command"].as<std::string>();
  if (sub_command == "ls") {
    options_description ls_desc("ls options");
    ls_desc.add_options()("hidden", "Show hidden files")(
      "path", value<std::string>(), "Path to list");

    std::vector<std::string> opts = collect_unrecognized(parsed.options, include_positional);
    opts.erase(opts.begin());

    // Parse again...
    store(command_line_parser(opts).options(ls_desc).run(), vm);

    //    "xsd", value<std::string>()->default_value(""), "XML Schema file(.xsd) for OpenSCENARIO")(
  }
  auto preprocessor = openscenario_preprocessor::Preprocessor();

  return 0;
}
