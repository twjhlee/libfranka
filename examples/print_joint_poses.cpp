// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#include <cmath>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <csignal>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <algorithm>
#include <ctime>
#include <franka/exception.h>
#include <franka/model.h>

/**
 * @example print_joint_poses.cpp
 * An example showing how to use the model library that prints the transformation
 * matrix of each joint with respect to the base frame.
 */

// template <class T, size_t N>
// std::ostream& operator<<(std::ostream& ostream, const std::array<T, N>& array) {
//   ostream << "[";
//   std::copy(array.cbegin(), array.cend() - 1, std::ostream_iterator<T>(ostream, ","));
//   std::copy(array.cend() - 1, array.cend(), std::ostream_iterator<T>(ostream));
//   ostream << "]";
//   return ostream;
// }

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <robot-hostname>" << " <save file name>" << std::endl;
    return -1;
  }

  try {
    //Textfile to save robot joint position
    char fname[100];
    sprintf(fname, "captured_trajectories/%s", argv[2]);
    std::ofstream pose_file (fname);

    franka::Robot robot(argv[1]);

    // Scan from keyboard input
    int choice = 1;
    int one = 1;
    std::cout << "Press 1 to record" << std::endl;
    while(choice){
        std::cin >> choice;
        // let's record trajectory
        if(choice == one){
            std::cout << "capture" << std::endl;
            franka::RobotState robot_state = robot.readOnce();
            std::array<double, 7> joint_pos;
            joint_pos = robot_state.q;
            for (int count = 0; count < 7; count ++){
                pose_file << joint_pos[count] << " ";
            }
            pose_file << std::endl;
        }
        // exit
        else{
            return 0;
        }

    }
    // franka::Model model(robot.loadModel());
    // for (franka::Frame frame = franka::Frame::kJoint1; frame <= franka::Frame::kEndEffector;
    //     frame++) {
    // std::cout << model.pose(frame, robot_state) << std::endl;
    //}
  } catch (franka::Exception const& e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  return 0;
}
