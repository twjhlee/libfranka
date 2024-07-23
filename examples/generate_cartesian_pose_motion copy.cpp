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
		float value;
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
  if (argc != 4) {
	std::cerr << "Usage: " << argv[0] << " <robot-hostname>"  << argv[1] << "<pose.txt path>" << argv[2] << "operation time" << std::endl;
	return -1;
  }

  try {
	franka::Robot robot(argv[1]);
	setDefaultBehavior(robot);

	// Read target pose of the end effector
	std::array<double, 16> target_pose = readFloatArrayFromFile(argv[2]);

	// Set additional parameters always before the control loop, NEVER in the control loop!
	// Set collision behavior.
	robot.setCollisionBehavior(
		{{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
		{{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
		{{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{40.0, 40.0, 40.0, 50.0, 50.0, 50.0}},
		{{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{40.0, 40.0, 40.0, 50.0, 50.0, 50.0}});

	std::array<double, 16> initial_pose;
	double time = 0.0;

	//init SE3
	//1  0  0  0.30689
	//0 -1  0  0
	//0  0 -1  0.486882
	//0  0  0  1

	// Get operation_time
	double oper_time = std::stod(argv[3]);

	robot.control([&time, &initial_pose, &oper_time, &target_pose](const franka::RobotState& robot_state,
										 franka::Duration period) -> franka::CartesianPose {
		time += period.toSec();

		if (time == 0.0) {
			initial_pose = robot_state.O_T_EE_c;
	  	}
	  	// Control to target pose - cosine curve
		double delta;
	  	double angle = (1 - std::cos(M_PI / oper_time * time)) * 0.5;
		std::array<double, 16> new_pose = initial_pose;
		for (int idx = 0; idx < 16; idx ++){
			delta = target_pose[idx] - initial_pose[idx];
			new_pose[idx] = initial_pose[idx] + (angle * delta);
		}

		// End condition
		if (time >= oper_time){
			std::cout << "Finished motion!" << std::endl;
			return franka::MotionFinished(new_pose);
		}
		return new_pose;
  	});
  	}
  	catch (const franka::Exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
  	}

  	return 0;
}
