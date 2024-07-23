// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <csignal>

#include <franka/exception.h>
#include <franka/robot.h>

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
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <robot-hostname>" << std::endl;
        return -1;
    }
    try {

        // read good trajectory
        std::vector<std::array<double, 7>> joint_traj = readFloatArrayFromFile("captured_trajectories/trial_3.txt");
        franka::Robot robot(argv[1]);
        setDefaultBehavior(robot);

        // First move the robot to a suitable joint configuration
        std::array<double, 7> q_goal = {{0, -M_PI_4, 0, -3 * M_PI_4, 0, M_PI_2, M_PI_4}};
        MotionGenerator motion_generator(0.3, q_goal);
        robot.control(motion_generator);

        // Set additional parameters always before the control loop, NEVER in the control loop!
        // Set collision behavior.
        robot.setCollisionBehavior(
            {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
            {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
            {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}},
            {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}});

        // File to save ee pose
        char fname[100];
        sprintf(fname, "%s/cam_transforms.txt", argv[2]);
        std::ofstream pose_file (fname);

        // Follow trajectory
        for (int idx=0; idx < joint_traj.size(); idx ++){
            if (idx > 0){
                break;
            }
            else{
                robot.control(MotionGenerator(0.2, joint_traj[idx]));
            }

        }

    }
    catch (const franka::Exception& e) {
        std::cout << e.what() << std::endl;
        // return -1;
    }
    catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        // return -1;
    }

}
