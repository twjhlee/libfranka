# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/panda/libfranka

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/panda/libfranka

# Include any dependencies generated for this target.
include examples/CMakeFiles/go_to_first_pose.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/go_to_first_pose.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/go_to_first_pose.dir/flags.make

examples/CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.o: examples/CMakeFiles/go_to_first_pose.dir/flags.make
examples/CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.o: examples/go_to_first_pose.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/panda/libfranka/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.o"
	cd /home/panda/libfranka/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.o -c /home/panda/libfranka/examples/go_to_first_pose.cpp

examples/CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.i"
	cd /home/panda/libfranka/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/panda/libfranka/examples/go_to_first_pose.cpp > CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.i

examples/CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.s"
	cd /home/panda/libfranka/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/panda/libfranka/examples/go_to_first_pose.cpp -o CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.s

# Object files for target go_to_first_pose
go_to_first_pose_OBJECTS = \
"CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.o"

# External object files for target go_to_first_pose
go_to_first_pose_EXTERNAL_OBJECTS =

examples/go_to_first_pose: examples/CMakeFiles/go_to_first_pose.dir/go_to_first_pose.cpp.o
examples/go_to_first_pose: examples/CMakeFiles/go_to_first_pose.dir/build.make
examples/go_to_first_pose: examples/libexamples_common.a
examples/go_to_first_pose: libfranka.so.0.8.0
examples/go_to_first_pose: examples/CMakeFiles/go_to_first_pose.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/panda/libfranka/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable go_to_first_pose"
	cd /home/panda/libfranka/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/go_to_first_pose.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/go_to_first_pose.dir/build: examples/go_to_first_pose

.PHONY : examples/CMakeFiles/go_to_first_pose.dir/build

examples/CMakeFiles/go_to_first_pose.dir/clean:
	cd /home/panda/libfranka/examples && $(CMAKE_COMMAND) -P CMakeFiles/go_to_first_pose.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/go_to_first_pose.dir/clean

examples/CMakeFiles/go_to_first_pose.dir/depend:
	cd /home/panda/libfranka && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/panda/libfranka /home/panda/libfranka/examples /home/panda/libfranka /home/panda/libfranka/examples /home/panda/libfranka/examples/CMakeFiles/go_to_first_pose.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/go_to_first_pose.dir/depend

