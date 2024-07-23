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
  if (argc != 2) {
    std::cerr << "Usage: ./grasp_object <gripper-hostname> <homing> <object-width>" << std::endl;
    return -1;
  }

  try {
    franka::Gripper gripper(argv[1]);
      gripper.homing();
  } catch (franka::Exception const& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  return 0;
}
