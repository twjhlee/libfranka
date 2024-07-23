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

/**
 * @example generate_joint_position_motion.cpp
 * An example showing how to generate a joint position motion.
 *
 * @warning Before executing this example, make sure there is enough space in front of the robot.
 */

std::vector<std::array<double, 7>> readFloatArrayFromFile(char* filename_ptr) {
    std::string filename(filename_ptr);
    std::vector<std::array<double, 7>> result;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::stringstream ss(line);
            float value;
            std::array<double, 7> arr;
            int idx = 0;
            while (ss >> value) {
                if (idx >= 7) continue;
                arr[idx++] = value;
            }
            result.push_back(arr);
        }
        file.close();
    }
    return result;
}

int main(int argc, char** argv) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " <robot-hostname>" << " <trajectory-filename>" << " <time>" << " <init pose>" << std::endl;
    return -1;
  }
  try {
    franka::Robot robot(argv[1]);
    // setDefaultBehavior(robot);

    // // First move the robot to a suitable joint configuration
    // std::cout << argv[4] << std::endl;
    // if (atoi(argv[4]) > 0){
    //     std::array<double, 7> q_goal = {{0, -M_PI_4, 0, -3 * M_PI_4, 0, M_PI_2, M_PI_4}};
    //     MotionGenerator motion_generator(0.5, q_goal);
    //     std::cout << "WARNING: This example will move the robot! "
    //             << "Please make sure to have the user stop button at hand!" << std::endl;
    //             //<< "Press Enter to continue..." << std::endl;
    //     //std::cin.ignore();
    //     robot.control(motion_generator);
    // }
    // std::cout << "Finished moving to initial joint configuration." << std::endl;
    std::vector<std::array<double, 7>> joint_traj = readFloatArrayFromFile(argv[2]);

    // Set additional parameters always before the control loop, NEVER in the control loop!
    // Set collision behavior.
    // robot.setCollisionBehavior(
    //     {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
    //     {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
    //     {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}},
    //     {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}});
    robot.setCollisionBehavior(
        {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0}},
        {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0}},
        {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{40.0, 40.0, 40.0, 50.0, 50.0, 50.0}},
        {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{40.0, 40.0, 40.0, 50.0, 50.0, 50.0}});

    // robot.setJointImpedance({{3000, 3000,3000,3000,3000,3000,3000}});

    robot.control(MotionGenerator(0.3, joint_traj[0]));
    std::cout << "Finished moving to initial joint configuration of trajectory." << std::endl;
    // robot.control([&motion_idx, &in_joint_traj](const franka::RobotState& robot_state,
    //                                          franka::Duration period) -> franka::JointPositions {
    //     auto has_arrived = [](const std::array<double, 7> p1, const std::array<double, 7> p2, double tolerance) {
    //         for (int i = 0; i < 7; ++i)
    //             if (abs(p1[i] - p2[i]) > tolerance) return false;
    //         return true;
    //     };

    //     // extend joint "from" "to"
    //     auto extend = [](const std::array<double, 7> from, const std::array<double, 7> to, double max_diff) {
    //         std::array<double, 7> result = {{0, 0, 0, 0, 0, 0, 0}};
    //         for (int i = 0; i < 7; ++i) {
    //             double diff = to[i] - from[i];
    //             std::cout << diff << std::endl;
    //             if (diff < -max_diff) diff = -max_diff;
    //             else if (diff > max_diff) diff = max_diff;

    //             // if (diff < 0.00000001) diff = 0;

    //             result[i] = from[i] + diff;
    //         }

    //         return (franka::JointPositions) result;
    //     };

    //     // robot_state.q_d; // current joint state
    //     std::array<double, 7> cur_joint_positions = robot_state.q_d;
    //     if (has_arrived(cur_joint_positions, in_joint_traj[motion_idx], 0.0005)) {
    //         std::cout << "arrived at joint position " << motion_idx << std::endl;
    //         return (franka::JointPositions) in_joint_traj[motion_idx++];
    //     }

    //     std::cout << "extend to " << motion_idx << std::endl;
    //     return extend(cur_joint_positions, in_joint_traj[motion_idx], 0.00005);
    // });
    double time = 0.0;
    double speed = std::stod(argv[4]);
    int motion_idx = 0;
    double T = std::stod(argv[3]);
    auto control_callback_gen = [&motion_idx, &time, &speed, &T](const std::vector<std::array<double, 7>> joint_traj) {
        return [&motion_idx, &joint_traj, &time, &speed, &T](const franka::RobotState& robot_state,
                                             franka::Duration period) -> franka::JointPositions {
            int last_traj_idx = joint_traj.size() - 1;
            time += period.toSec();


            double motion_time = last_traj_idx * (1.0 - std::cos(M_PI * time / T))/ 2.0;
            motion_idx = (int)motion_time;

            // at the end
            if (motion_idx >= last_traj_idx) {
                std::cout << std::endl << "Finished motion, shutting down example" << std::endl;
                franka::JointPositions output = joint_traj[motion_idx];
                // franka::JointPositions output = robot_state.q_d;
                return franka::MotionFinished(output);
            }

            std::array<double, 7> target = {{0, 0, 0, 0, 0, 0, 0}};
            for (int i = 0; i < 7; ++i) {
                target[i] += joint_traj[motion_idx][i] * (motion_idx + 1 - motion_time) +
                        joint_traj[motion_idx + 1][i] * (motion_time - motion_idx);
            }
            franka::JointPositions output = target;

            return output;
        };
    };
    // original version
    // robot.setJointImpedance({{3000, 3000, 3000, 3000, 3000, 3000, 3000}});
    robot.control(control_callback_gen(joint_traj));
    // cartesion impedance controller
    //robot.setCartesianImpedance({{2000, 2000, 2000, 100, 100, 100}});
    // robot.control(control_callback_gen(joint_traj), franka::ControllerMode::kCartesianImpedance);

    // TODO: Get grasping width from two points
    //double grasping_width = 0.01;
    //if (!gripper.grasp(grasping_width, 0.1, 60)) {
    //    std::cout << "Failed to grasp object." << std::endl;
    //}
    //robot.control(control_callback_gen(out_joint_traj));

    // robot.control([&in_joint_traj, &time, &speed](const franka::RobotState& robot_state,
    //                                          franka::Duration period) -> franka::JointPositions {
    //     time += period.toSec();
    //     double motion_time = time * speed;

    //     if (time == 0.0) {
    //         std::cout << std::endl << "initial_position: ";
    //         for (int i = 0; i < 7; ++i) {
    //             std::cout << robot_state.q_d[i] << ", ";
    //         }
    //         std::cout << std::endl;
    //         std::cout << std::endl << "first joint position: ";
    //         for (int i = 0; i < 7; ++i) {
    //             std::cout << in_joint_traj[0][i] << ", ";
    //         }
    //         std::cout << std::endl;
    //     }

    //     int last_traj_idx = in_joint_traj.size();
    //     int motion_idx = (int)motion_time;

    //     if (motion_idx >= last_traj_idx - 1) {
    //         std::cout << std::endl << "Finished motion, shutting down example" << std::endl;
    //         franka::JointPositions output = in_joint_traj[motion_idx - 1];
    //         return franka::MotionFinished(output);
    //     }

    //     std::array<double, 7> target = {{0, 0, 0, 0, 0, 0, 0}};
    //     for (int i = 0; i < 7; ++i) {
    //         target[i] += in_joint_traj[motion_idx][i] * (motion_idx + 1 - motion_time) + joint_traj[motion_idx + 1][i] * (motion_time - motion_idx);
    //     }
    //     std::cout << std::endl << "target position: ";
    //     for (int i = 0; i < 7; ++i) {
    //         std::cout << target[i] << ", ";
    //     }
    //     std::cout << std::endl;

    //     franka::JointPositions output = target;

    //     return output;
    // });
  } catch (const franka::Exception& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  return 0;
}
