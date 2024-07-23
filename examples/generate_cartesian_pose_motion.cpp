// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#include <cmath>
#include <iostream>
#include <fstream>
#include <math.h>

#include <franka/exception.h>
#include <franka/robot.h>

#include "examples_common.h"

/**
 * @example generate_cartesian_pose_motion.cpp
 * An example showing how to generate a Cartesian motion.
 *
 * @warning Before executing this example, make sure there is enough space in front of the robot.
 */
std::array<double, 16> readFloatArrayFromFile(char* filename_ptr) {
	std::string filename(filename_ptr);
	std::array<double, 16> result;
	std::ifstream file(filename);
	if (file.is_open()) {
		std::string line;
		std::getline(file, line);
		std::stringstream ss(line);
		double value;
		int idx = 0;
		while (ss >> value) {
			if (idx >= 16) continue;
			result[idx++] = value;
		}
		file.close();
	}
	return result;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <robot-hostname>" << " <target-pose-path>" << std::endl;
    return -1;
  }
  try {
    franka::Robot robot(argv[1]);
    setDefaultBehavior(robot);

    // First move the robot to a suitable joint configuration
    std::array<double, 7> q_goal = {{0, -M_PI_4, 0, -3 * M_PI_4, 0, M_PI_2, M_PI_4}};
    MotionGenerator motion_generator(0.5, q_goal);
    std::cout << "WARNING: This example will move the robot! "
              << "Please make sure to have the user stop button at hand!" << std::endl
              << "Press Enter to continue..." << std::endl;
    std::cin.ignore();
    robot.control(motion_generator);
    std::cout << "Finished moving to initial joint configuration." << std::endl;

    // Read target pose of the end effector
	std::array<double, 16> target_pose = readFloatArrayFromFile(argv[2]);
    for (const auto& e: target_pose) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    // Set additional parameters always before the control loop, NEVER in the control loop!
    // Set collision behavior.
    robot.setCollisionBehavior(
        {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
        {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
        {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}},
        {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}});

    std::array<double, 16> initial_pose;
    double time = 0.0;
    robot.control([&time, &initial_pose, &target_pose](const franka::RobotState& robot_state,
                                         franka::Duration period) -> franka::CartesianPose {
        time += period.toSec();

        if (time == 0.0) {
            initial_pose = (std::array<double, 16>)robot_state.O_T_EE_c;
        }

        // constexpr double kRadius = 0.3;
        // double angle = M_PI / 4 * (1 - std::cos(M_PI / 5.0 * time));
        // double delta_x = kRadius * std::sin(angle);
        // double delta_z = kRadius * (std::cos(angle) - 1);

        // std::array<double, 16> new_pose = initial_pose;
        // new_pose[12] += delta_x;
        // new_pose[14] += delta_z;

        double delta;
        double angle = (1 - std::cos(M_PI / 10 * time)) * 0.5;
        std::array<double, 16> new_pose = {0.0};
        for (int idx = 0; idx < 16; idx ++){
            delta = target_pose[idx] - initial_pose[idx];
            new_pose[idx] = initial_pose[idx] + (angle * delta);
        }

      if (time >= 10.0) {
        std::cout << std::endl << "Finished motion, shutting down example" << std::endl;
        return franka::MotionFinished(new_pose);
      }
      return new_pose;
    });
  } catch (const franka::Exception& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  return 0;
}