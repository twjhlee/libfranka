// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include <franka/exception.h>
#include <franka/gripper.h>

/**
 * @example grasp_object.cpp
 * An example showing how to control FRANKA's gripper.
 */

int main(int argc, char** argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./grasp_object <gripper-hostname> <homing> <object-width>" << std::endl;
    return -1;
  }

  try {
    franka::Gripper gripper(argv[1]);
    double grasping_width = std::stod(argv[3]);

    std::stringstream ss(argv[2]);
    bool homing;
    if (!(ss >> homing)) {
      std::cerr << "<homing> can be 0 or 1." << std::endl;
      return -1;
    }

    if (homing) {
      // Do a homing in order to estimate the maximum grasping width with the current fingers.
      gripper.homing();
    }

    // Check for the maximum grasping width.
    franka::GripperState gripper_state = gripper.readOnce();

    std::cout << "width : " << gripper_state.width << std::endl;
    std::cout << "max_width : " << gripper_state.max_width << std::endl;
    std::cout << "grasping_width : " << grasping_width << std::endl;

    // if (gripper_state.max_width < grasping_width) {
    //   std::cout << "Object is too large for the current fingers on the gripper." << std::endl;
    //   return -1;
    // }

    // Grasp the object.
    if (!gripper.grasp(grasping_width, 0.1, 60, 0.1, 0.1)) {
      std::cout << "Failed to grasp object." << std::endl;
      return -1;
    }

    // Wait 3s and check afterwards, if the object is still grasped.
    // std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));

    gripper_state = gripper.readOnce();
    if (!gripper_state.is_grasped) {
      std::cout << "Object lost." << std::endl;
      return -1;
    }

    std::cout << "Grasped object, will release it now." << std::endl;
    //gripper.stop();
  } catch (franka::Exception const& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  return 0;
}
