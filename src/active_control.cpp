// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE

#include <franka/active_control.h>
#include <franka/exception.h>
#include <franka/robot.h>

#include "robot_impl.h"

namespace franka {

ActiveControl::ActiveControl(std::shared_ptr<Robot::Impl> robot_impl,
                             uint32_t motion_id,
                             std::unique_lock<std::mutex> control_lock,
                             ActiveControlControllerOptions controller_type,
                             ActiveControlMotionGeneratorOptions motion_generator_type)
    : robot_impl_(std::move(robot_impl)),
      motion_id_(motion_id),
      control_lock_(std::move(control_lock)),
      control_finished_(false),
      first_read_attempt_(true),
      controller_type_(controller_type),
      motion_generator_type_(motion_generator_type) {}

ActiveControl::~ActiveControl() {
  if (!control_finished_) {
    robot_impl_->cancelMotion(motion_id_);
  }
}

void ActiveControl::writeOnce(const Torques& control_input) {
  if (controller_type_ != ActiveControlControllerOptions::kExternalController) {
    throw franka::ControlException("writeOnce called for wrong control mode.");
  }

  if (control_finished_) {
    throw franka::ControlException("writeOnce must not be called after the motion has finished.");
  }

  if (control_input.motion_finished) {
    control_finished_ = control_input.motion_finished;
    robot_impl_->finishMotion(motion_id_, control_input);
    control_lock_.unlock();
    return;
  }

  robot_impl_->writeOnce(control_input);
}

void ActiveControl::writeOnce(const JointPositions& motion_generator_input) {
  if ((motion_generator_type_ == ActiveControlMotionGeneratorOptions::kJointPosition) &&
      (controller_type_ == ActiveControlControllerOptions::kExternalController)) {
    throw franka::ControlException("writeOnce called for wrong motion generator or control mode.");
  }

  if (control_finished_) {
    throw franka::ControlException("writeOnce must not be called after the motion has finished.");
  }

  throw franka::ControlException("Not yet implemented!");
}

void ActiveControl::writeOnce(const JointVelocities& motion_generator_input) {
  if ((motion_generator_type_ == ActiveControlMotionGeneratorOptions::kJointVelocity) &&
      (controller_type_ == ActiveControlControllerOptions::kExternalController)) {
    throw franka::ControlException("writeOnce called for wrong motion generator or control mode.");
  }

  if (control_finished_) {
    throw franka::ControlException("writeOnce must not be called after the motion has finished.");
  }

  throw franka::ControlException("Not yet implemented!");
}

void ActiveControl::writeOnce(const CartesianPose& motion_generator_input) {
  if ((motion_generator_type_ == ActiveControlMotionGeneratorOptions::kCartesianPosition) &&
      (controller_type_ == ActiveControlControllerOptions::kExternalController)) {
    throw franka::ControlException("writeOnce called for wrong motion generator or control mode.");
  }

  if (control_finished_) {
    throw franka::ControlException("writeOnce must not be called after the motion has finished.");
  }

  throw franka::ControlException("Not yet implemented!");
}

void ActiveControl::writeOnce(const CartesianVelocities& motion_generator_input) {
  if ((motion_generator_type_ == ActiveControlMotionGeneratorOptions::kCartesianVelocity) &&
      (controller_type_ == ActiveControlControllerOptions::kExternalController)) {
    throw franka::ControlException("writeOnce called for wrong motion generator or control mode.");
  }

  throw franka::ControlException("Not yet implemented!");
}

void ActiveControl::writeOnce(const JointPositions& motion_generator_input,
                              const Torques& control_input) {
  if ((motion_generator_type_ == ActiveControlMotionGeneratorOptions::kJointPosition) &&
      (controller_type_ != ActiveControlControllerOptions::kExternalController)) {
    throw franka::ControlException("writeOnce called for wrong motion generator or control mode.");
  }

  if (control_finished_) {
    throw franka::ControlException("writeOnce must not be called after the motion has finished.");
  }

  throw franka::ControlException("Not yet implemented!");
}

void ActiveControl::writeOnce(const JointVelocities& motion_generator_input,
                              const Torques& control_input) {
  if ((motion_generator_type_ == ActiveControlMotionGeneratorOptions::kJointVelocity) &&
      (controller_type_ != ActiveControlControllerOptions::kExternalController)) {
    throw franka::ControlException("writeOnce called for wrong motion generator or control mode.");
  }

  if (control_finished_) {
    throw franka::ControlException("writeOnce must not be called after the motion has finished.");
  }

  throw franka::ControlException("Not yet implemented!");
}

void ActiveControl::writeOnce(const CartesianPose& motion_generator_input,
                              const Torques& control_input) {
  if ((motion_generator_type_ == ActiveControlMotionGeneratorOptions::kCartesianPosition) &&
      (controller_type_ != ActiveControlControllerOptions::kExternalController)) {
    throw franka::ControlException("writeOnce called for wrong motion generator or control mode.");
  }

  if (control_finished_) {
    throw franka::ControlException("writeOnce must not be called after the motion has finished.");
  }

  throw franka::ControlException("Not yet implemented!");
}

void ActiveControl::writeOnce(const CartesianVelocities& motion_generator_input,
                              const Torques& control_input) {
  if ((motion_generator_type_ == ActiveControlMotionGeneratorOptions::kCartesianVelocity) &&
      (controller_type_ != ActiveControlControllerOptions::kExternalController)) {
    throw franka::ControlException("writeOnce called for wrong motion generator or control mode.");
  }

  if (control_finished_) {
    throw franka::ControlException("writeOnce must not be called after the motion has finished.");
  }

  throw franka::ControlException("Not yet implemented!");
}

std::pair<RobotState, Duration> ActiveControl::readOnce() {
  auto robot_state = robot_impl_->readOnce();
  robot_impl_->throwOnMotionError(robot_state, motion_id_);

  if (first_read_attempt_) {
    first_read_attempt_ = false;
    last_read_access_ = robot_state.time;
    return std::make_pair(robot_state, Duration(0));
  }

  auto time_since_last_read = robot_state.time - last_read_access_;
  last_read_access_ = robot_state.time;

  return std::make_pair(robot_state, time_since_last_read);
}

}  // namespace franka