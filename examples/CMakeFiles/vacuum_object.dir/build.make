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
include examples/CMakeFiles/vacuum_object.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/vacuum_object.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/vacuum_object.dir/flags.make

examples/CMakeFiles/vacuum_object.dir/vacuum_object.cpp.o: examples/CMakeFiles/vacuum_object.dir/flags.make
examples/CMakeFiles/vacuum_object.dir/vacuum_object.cpp.o: examples/vacuum_object.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/panda/libfranka/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/vacuum_object.dir/vacuum_object.cpp.o"
	cd /home/panda/libfranka/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vacuum_object.dir/vacuum_object.cpp.o -c /home/panda/libfranka/examples/vacuum_object.cpp

examples/CMakeFiles/vacuum_object.dir/vacuum_object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vacuum_object.dir/vacuum_object.cpp.i"
	cd /home/panda/libfranka/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/panda/libfranka/examples/vacuum_object.cpp > CMakeFiles/vacuum_object.dir/vacuum_object.cpp.i

examples/CMakeFiles/vacuum_object.dir/vacuum_object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vacuum_object.dir/vacuum_object.cpp.s"
	cd /home/panda/libfranka/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/panda/libfranka/examples/vacuum_object.cpp -o CMakeFiles/vacuum_object.dir/vacuum_object.cpp.s

# Object files for target vacuum_object
vacuum_object_OBJECTS = \
"CMakeFiles/vacuum_object.dir/vacuum_object.cpp.o"

# External object files for target vacuum_object
vacuum_object_EXTERNAL_OBJECTS =

examples/vacuum_object: examples/CMakeFiles/vacuum_object.dir/vacuum_object.cpp.o
examples/vacuum_object: examples/CMakeFiles/vacuum_object.dir/build.make
examples/vacuum_object: examples/libexamples_common.a
examples/vacuum_object: libfranka.so.0.8.0
examples/vacuum_object: examples/CMakeFiles/vacuum_object.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/panda/libfranka/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable vacuum_object"
	cd /home/panda/libfranka/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vacuum_object.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/vacuum_object.dir/build: examples/vacuum_object

.PHONY : examples/CMakeFiles/vacuum_object.dir/build

examples/CMakeFiles/vacuum_object.dir/clean:
	cd /home/panda/libfranka/examples && $(CMAKE_COMMAND) -P CMakeFiles/vacuum_object.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/vacuum_object.dir/clean

examples/CMakeFiles/vacuum_object.dir/depend:
	cd /home/panda/libfranka && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/panda/libfranka /home/panda/libfranka/examples /home/panda/libfranka /home/panda/libfranka/examples /home/panda/libfranka/examples/CMakeFiles/vacuum_object.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/vacuum_object.dir/depend

