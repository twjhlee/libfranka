// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#include <cmath>
#include <iostream>
#include <fstream>
#include <math.h>

#include <franka/exception.h>
#include <franka/robot.h>
#include <franka/gripper.h>

#include "examples_common.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <robot-hostname>" << std::endl;
    return -1;
  }
  try {
    franka::Robot robot(argv[1]);
    setDefaultBehavior(robot);

    // Set additional parameters always before the control loop, NEVER in the control loop!
    // Set collision behavior.
    robot.setCollisionBehavior(
        {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
        {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
        {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}},
        {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}});

    while (true) {
        std::cout << "Enter mode [0: move, 1: gripper open, 2: gripper close, 3: move to init pose]: ";
        int mode;
        std::cin >> mode;

        if (mode == 0) {
            std::cout << "Enter gripper target pose [x y z]: ";
            // read input
            std::cin.ignore();
            std::string target_pose_str;
            std::getline(std::cin, target_pose_str);

            std::stringstream ss(target_pose_str);
            double value;
            std::vector<double> result;
            while (ss >> value) {
                result.push_back(value);
            }

            // validation check
            int n=result.size();
            if (n != 3) {
                // std::cout << "Argument should be the form of x y z!" << std::endl;
                break;
            }

            std::array<double, 16> target_pose = {1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, result[0], result[1], result[2], 1};
            std::array<double, 16> initial_pose;

            double MOVE_TIME = 5.0;
            double time = 0.0;
            robot.control([&time, &initial_pose, &target_pose, &MOVE_TIME](const franka::RobotState& robot_state,
                                                franka::Duration period) -> franka::CartesianPose {
                time += period.toSec();

                if (time == 0.0) {
                    initial_pose = (std::array<double, 16>)robot_state.O_T_EE_c;
                }

                double delta;
                double angle = (1 - std::cos(M_PI / MOVE_TIME * time)) * 0.5;
                std::array<double, 16> new_pose = {0.0};
                for (int idx = 0; idx < 16; idx ++){
                    delta = target_pose[idx] - initial_pose[idx];
                    new_pose[idx] = initial_pose[idx] + (angle * delta);
                }

                if (time >= MOVE_TIME) {
                    // std::cout << std::endl << "Finished motion." << std::endl;
                    return franka::MotionFinished(new_pose);
                }
                return new_pose;
            });
        }
        else if (mode == 1) {
            franka::Gripper gripper(argv[1]);
            gripper.homing();
        }
        else if (mode == 2) {
            franka::Gripper gripper(argv[1]);
            if (!gripper.grasp(0.01, 0.1, 60, 0.1, 0.1)) {
                std::cout << "Failed to grasp object." << std::endl;
            }
        }
        else if (mode == 3) {
            std::array<double, 7> q_goal = {{0, -M_PI_4, 0, -3 * M_PI_4, 0, M_PI_2, M_PI_4}};
            MotionGenerator motion_generator(0.2, q_goal);
            robot.control(motion_generator);
        }
        else {
            std::cout << "Get invalid input! finish the app." << std::endl;
            break;
        }
    }
    // Read target pose of the end effector


  } catch (const franka::Exception& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  return 0;
}