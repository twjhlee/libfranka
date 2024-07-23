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
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <algorithm>
#include <ctime>

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
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <robot-hostname>" <<  " <save-dir name>" << " <trajectory_file>" << "<capture_step>"<< std::endl;
        return -1;
    }
    try {
        // Create save directory
        char savedir[100];
        sprintf(savedir, "datasets/%s", argv[2]);
        mkdir(savedir, 0777);

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
        rs2::pipeline_profile pipeline_profile = p.start(cfg);
        rs2::device dev = pipeline_profile.get_device();
        std::vector<rs2::sensor> sensors = dev.query_sensors();

        // Change sensor option
        // float gamma = 300;
        float gamma = 300;
        rs2_option gamma_option = static_cast<rs2_option>(5);
        sensors[1].set_option(gamma_option, gamma);
        
        // float brightness = 0;
        float brightness = 5;
        rs2_option brightness_option = static_cast<rs2_option>(1);
        sensors[1].set_option(brightness_option, brightness);

        // float sharpness = 50;
        float sharpness = 70;
        rs2_option sharpness_option = static_cast<rs2_option>(8);
        sensors[1].set_option(sharpness_option, sharpness);

        float wb = 5500;
        rs2_option wb_option = static_cast<rs2_option>(9);
        sensors[1].set_option(wb_option, wb);
        
        // float autowb = 1;
        // rs2_option autowb_option = static_cast<rs2_option>(11);
        // sensors[1].set_option(autowb_option, autowb);


        // float contrast = 50;
        float contrast = 60;
        rs2_option contrast_option = static_cast<rs2_option>(2);
        sensors[1].set_option(contrast_option, contrast);


        // TODO: add for or while loop
        // Block program until frames arrive
        for (auto i = 0; i < 30; i++) p.wait_for_frames();
        // read good trajectory
        std::vector<std::array<double, 7>> joint_traj = readFloatArrayFromFile(argv[3]);
        // std::vector<std::array<double, 7>> joint_traj = readFloatArrayFromFile("trajectories/joint_trajs_sphere.txt");
        franka::Robot robot(argv[1]);
        setDefaultBehavior(robot);

        // First move the robot to a suitable joint configuration
        std::array<double, 7> q_goal = {{0, -M_PI_4, 0, -3 * M_PI_4, 0, M_PI_2, M_PI_4}};
        MotionGenerator motion_generator(0.3, q_goal);
        robot.control(motion_generator);

        // Set additional parameters always before the control loop, NEVER in the control loop!
        // Set collision behavior.
        robot.setCollisionBehavior(
            {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0}},
            {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0}},
            {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{40.0, 40.0, 40.0, 50.0, 50.0, 50.0}},
            {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{40.0, 40.0, 40.0, 50.0, 50.0, 50.0}});

        // File to save ee pose
        char fname[100];
        sprintf(fname, "datasets/%s/cam_transforms.txt", argv[2]);
        std::ofstream pose_file (fname);

        int c_step;
        c_step = atoi(argv[4]);
        int img_idx;
        img_idx = 0;
        // Follow trajectory
        for (uint idx=0; idx < joint_traj.size(); idx ++){
            if (idx % c_step == 0){
                
                // auto tick1 = std::chrono::system_clock::now();

                // std::cout << joint_traj[idx][0] << ", " << joint_traj[idx][1] << ", " << joint_traj[idx][2] << ", " << joint_traj[idx][3] << ", " << joint_traj[idx][4] << ", " << joint_traj[idx][5] << ", " << joint_traj[idx][6] << std::endl;
                robot.control(MotionGenerator(0.24, joint_traj[idx]));
                
                // auto tick2 = std::chrono::system_clock::now();

                // Maybe add some waiting time - Lets delete this?
                for (auto i = 0; i < 50; i++) p.wait_for_frames();
                rs2::frameset frameset = p.wait_for_frames();
                frameset = align_to_color.process(frameset);
                auto depth = frameset.get_depth_frame();
                auto color = frameset.get_color_frame();
                // Colorized code
                // auto colorized_depth = color_map.colorize(depth);
                std::stringstream depth_png_file;
                char dname[100];
                sprintf(dname, "datasets/%s/%06d-depth.bin", argv[2], img_idx);
                depth_png_file << dname;
                std::cout << dname << std::endl;
                std::ofstream depth_rawfile(dname, std::ofstream::binary);
                depth_rawfile.write(static_cast<const char*>(depth.get_data()), depth.get_height() * depth.get_stride_in_bytes());

                // stbi_write_png(depth_png_file.str().c_str(), colorized_depth.get_width(), colorized_depth.get_height(),
                //                 colorized_depth.get_bytes_per_pixel(), colorized_depth.get_data(), colorized_depth.get_stride_in_bytes());

                std::stringstream color_png_file;
                char fname[100];
                sprintf(fname, "datasets/%s/%06d-color.png", argv[2], img_idx);
                color_png_file << fname;
                std::cout << fname << std::endl;
                stbi_write_png(color_png_file.str().c_str(), color.get_width(), color.get_height(),
                                color.get_bytes_per_pixel(), color.get_data(), color.get_stride_in_bytes());

                // auto tick3 = std::chrono::system_clock::now();

                // Get ee pose
                franka::RobotState robot_state = robot.readOnce();
                std::array<double, 16> pose;
                pose = robot_state.O_T_EE;
                for (int count = 0; count < 16; count++){
                    pose_file << pose[count] << " ";
                }
                pose_file << std::endl;

                img_idx = img_idx + 1;

                // auto tick4 = std::chrono::system_clock::now();
                // std:chrono::duration<double> dur1 = tick2 - tick1;
                // std:chrono::duration<double> dur2 = tick3 - tick2;
                // std:chrono::duration<double> dur3 = tick4 - tick3;
                // std::cout << "tick 1: " << dur1.count() << " tick 2: " << dur2.count() << " tick 3: " << dur3.count() << std::endl;
                // for debug
            }
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
    char savedir[100];
    sprintf(savedir, "datasets/%s", argv[2]);
    mkdir(savedir, 0777);
    sprintf(command, "scp -r -P 7910 %s twjhlee@147.46.66.190:/home/twjhlee/Data_ssd/INMC/ICF", savedir);
    system(command);

    return 0;
}
