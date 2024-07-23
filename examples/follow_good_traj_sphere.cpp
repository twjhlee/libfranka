// Copyright (c) 2017 Franka Emika GmbH
// Use of this source code is governed by the Apache-2.0 license, see LICENSE
#include <cmath>
#include <iostream>
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
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
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <robot-hostname>" <<  " <save-dir name>" << std::endl;
        return -1;
    }
    try {
        // Create save directory
        mkdir(argv[2], 0777);

        // Create a Pipeline - this serves as a top-level API for streaming and processing frames
        rs2::colorizer color_map;
        rs2::pipeline p;
        rs2::config cfg;
        rs2::align align_to_color(RS2_STREAM_COLOR);

        // auto sensor = p.get_device().first<rs2::depth_sensor>();
        // auto depth_unit = sensor.get_option(RS2_OPTION_DEPTH_UNITS);
        // auto scale = sensor.get_depth_scale();
        // printf("[BEFORE] depth scale: %f\n", scale);
        // sensor.set_option(RS2_OPTION_DEPTH_UNITS, 0.0001);
        // depth_unit = sensor.get_option(RS2_OPTION_DEPTH_UNITS);
        // scale = sensor.get_depth_scale();
        // printf("[AFTER] depth scale: %f\n", scale);

        cfg.enable_stream(RS2_STREAM_DEPTH);
        cfg.enable_stream(RS2_STREAM_COLOR, 640, 480);

        // Configure and start the pipeline
        p.start(cfg);

        // TODO: add for or while loop
        // Block program until frames arrive
        for (auto i = 0; i < 30; i++) p.wait_for_frames();
        // read good trajectory
        std::vector<std::array<double, 7>> joint_traj = readFloatArrayFromFile("trajectories/joint_trajs_sphere.txt");
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
            if (idx == 51){
                robot.control(MotionGenerator(0.4, joint_traj[idx]));
            }
            else{
                robot.control(MotionGenerator(0.2, joint_traj[idx]));
            }
            // Maybe add some waiting time
            for (auto i = 0; i < 30; i++) p.wait_for_frames();
            rs2::frameset frameset = p.wait_for_frames();
            frameset = align_to_color.process(frameset);
            auto depth = frameset.get_depth_frame();
            auto color = frameset.get_color_frame();
            // Colorized code
            // auto colorized_depth = color_map.colorize(depth);
            std::stringstream depth_png_file;
            char dname[100];
            sprintf(dname, "%s/%06d-depth.bin", argv[2], idx);
            depth_png_file << dname;
            std::cout << dname << std::endl;
            std::ofstream depth_rawfile(dname, std::ofstream::binary);
            depth_rawfile.write(static_cast<const char*>(depth.get_data()), depth.get_height() * depth.get_stride_in_bytes());

            // stbi_write_png(depth_png_file.str().c_str(), colorized_depth.get_width(), colorized_depth.get_height(),
            //                 colorized_depth.get_bytes_per_pixel(), colorized_depth.get_data(), colorized_depth.get_stride_in_bytes());

            std::stringstream color_png_file;
            char fname[100];
            sprintf(fname, "%s/%06d-color.png", argv[2], idx);
            color_png_file << fname;
            std::cout << fname << std::endl;
            stbi_write_png(color_png_file.str().c_str(), color.get_width(), color.get_height(),
                            color.get_bytes_per_pixel(), color.get_data(), color.get_stride_in_bytes());

            // Get ee pose
            franka::RobotState robot_state = robot.readOnce();
            std::array<double, 16> pose;
            pose = robot_state.O_T_EE;
            for (int count = 0; count < 16; count++){
                pose_file << pose[count] << " ";
            }
            pose_file << std::endl;

        }

        // First move the robot to a suitable joint configuration
        MotionGenerator motion_generator_init(0.3, q_goal);
        robot.control(motion_generator_init);


    }
    catch (const franka::Exception& e) {
        std::cout << e.what() << std::endl;
        // return -1;
    }
    catch (const rs2::error & e){
        std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
        // return -1;
    }
    catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        // return -1;
    }

    char command[200];
    // sprintf(command, "python convert_rs2np.py --input_dir %s", argv[2]);
    // system(command);
    sprintf(command, "scp -r -P 7910 %s twjhlee@172.16.0.3:/home/twjhlee/Data_ssd/INMC/0404", argv[2]);
    system(command);

    return 0;
}
