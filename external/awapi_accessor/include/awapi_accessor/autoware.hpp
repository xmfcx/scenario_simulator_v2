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

#ifndef AWAPI_ACCESSOR__AUTOWARE_HPP_
#define AWAPI_ACCESSOR__AUTOWARE_HPP_

#include <bits/c++config.h>
#define AUTOWARE_IV
// #define AUTOWARE_AUTO

#define AWAPI_CONCEALER_ISOLATE_STANDARD_OUTPUT false

#include <sys/wait.h>

#include <awapi_accessor/continuous_transform_broadcaster.hpp>
#include <awapi_accessor/fundamental_api.hpp>
#include <awapi_accessor/launch.hpp>
#include <awapi_accessor/miscellaneous_api.hpp>
#include <awapi_accessor/transition_assertion.hpp>
#include <awapi_accessor/utility/visibility.hpp>
#include <mutex>

namespace awapi
{
class Autoware : public rclcpp::Node,
                 public ContinuousTransformBroadcaster<Autoware>,
                 public FundamentalAPI<Autoware>,
                 public MiscellaneousAPI<Autoware>,
                 public TransitionAssertion<Autoware>
{
  friend class ContinuousTransformBroadcaster<Autoware>;
  friend class FundamentalAPI<Autoware>;
  friend class MiscellaneousAPI<Autoware>;
  friend class TransitionAssertion<Autoware>;

  std::mutex mutex;

  decltype(auto) lock() { return std::unique_lock<std::mutex>(mutex); }

  const pid_t process_id;

  std::promise<void> promise;

  std::thread spinner;

  const rclcpp::TimerBase::SharedPtr continuous_update;

  geometry_msgs::msg::Pose current_pose;
  geometry_msgs::msg::Twist current_twist;

  void update()
  {
    setCurrentControlMode();
    setCurrentPose(current_pose);
    setCurrentShift(current_twist);
    setCurrentSteering(current_twist);
    setCurrentTurnSignal();
    setCurrentTwist(current_twist);
    setCurrentVelocity(current_twist);
    setLaneChangeApproval();
    setLocalizationTwist(current_twist);
    setTransform(current_pose);
    // setVehicleVelocity(parameters.performance.max_speed);
  }

public:
  template <std::size_t Rate = 5, typename... Ts>
  AWAPI_ACCESSOR_PUBLIC explicit constexpr Autoware(Ts &&... xs)
  : rclcpp::Node("awapi_accessor", "simulation", rclcpp::NodeOptions().use_global_arguments(false)),
    process_id(ros2_launch(std::forward<decltype(xs)>(xs)...)),
    spinner(
      [this](auto future) {
        while (rclcpp::ok() and
               future.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
          rclcpp::spin_some(get_node_base_interface());
        }
      },
      std::move(promise.get_future())),
    continuous_update(
      create_wall_timer(std::chrono::milliseconds(Rate), [this]() { return update(); }))
  {
  }

  virtual ~Autoware()
  {
    if (spinner.joinable()) {
      promise.set_value();
      spinner.join();
    }

    int status = 0;

    if (::kill(process_id, SIGINT) < 0 or ::waitpid(process_id, &status, WUNTRACED) < 0) {
      std::cout << std::system_error(errno, std::system_category()).what() << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  const auto & set(const geometry_msgs::msg::Pose & pose) { return current_pose = pose; }

  const auto & set(const geometry_msgs::msg::Twist & twist) { return current_twist = twist; }

  void initialize(const geometry_msgs::msg::Pose & initial_pose)
  {
    set(initial_pose);

    waitForAutowareStateToBeInitializingVehicle();

    waitForAutowareStateToBeWaitingForRoute([&]() {  //
      setInitialPose(initial_pose);
    });
  }

  // TODO(yamacir-kit) PoseStamped => Pose
  void drive(
    const geometry_msgs::msg::PoseStamped & destination,
    const std::vector<geometry_msgs::msg::PoseStamped> & checkpoints = {})
  {
    // NOTE: This is assertion.
    waitForAutowareStateToBeWaitingForRoute();

    auto request = [&]() {
      setGoalPose(destination);
      for (const auto & checkpoint : checkpoints) {
        setCheckpoint(checkpoint);
      }
    };

    // NOTE: Autoware.IV waits about 3 sec from the completion of Planning until the transition to WaitingForEngage.
    waitForAutowareStateToBePlanning(request, std::chrono::seconds(5));

    waitForAutowareStateToBeWaitingForEngage();
  }

  void engage()
  {
    waitForAutowareStateToBeDriving([this]() { setAutowareEngage(true); });
  }
};
}  // namespace awapi

#include <awapi_accessor/undefine_macro.hpp>

#endif  // AWAPI_ACCESSOR__AUTOWARE_HPP_
