# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/oscar/github/nes

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oscar/github/nes/build

# Include any dependencies generated for this target.
include CMakeFiles/nes.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/nes.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/nes.dir/flags.make

CMakeFiles/nes.dir/nes/src/cpu.cpp.o: CMakeFiles/nes.dir/flags.make
CMakeFiles/nes.dir/nes/src/cpu.cpp.o: ../nes/src/cpu.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oscar/github/nes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/nes.dir/nes/src/cpu.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/nes.dir/nes/src/cpu.cpp.o -c /home/oscar/github/nes/nes/src/cpu.cpp

CMakeFiles/nes.dir/nes/src/cpu.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nes.dir/nes/src/cpu.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oscar/github/nes/nes/src/cpu.cpp > CMakeFiles/nes.dir/nes/src/cpu.cpp.i

CMakeFiles/nes.dir/nes/src/cpu.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nes.dir/nes/src/cpu.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oscar/github/nes/nes/src/cpu.cpp -o CMakeFiles/nes.dir/nes/src/cpu.cpp.s

CMakeFiles/nes.dir/nes/src/cpu.cpp.o.requires:

.PHONY : CMakeFiles/nes.dir/nes/src/cpu.cpp.o.requires

CMakeFiles/nes.dir/nes/src/cpu.cpp.o.provides: CMakeFiles/nes.dir/nes/src/cpu.cpp.o.requires
	$(MAKE) -f CMakeFiles/nes.dir/build.make CMakeFiles/nes.dir/nes/src/cpu.cpp.o.provides.build
.PHONY : CMakeFiles/nes.dir/nes/src/cpu.cpp.o.provides

CMakeFiles/nes.dir/nes/src/cpu.cpp.o.provides.build: CMakeFiles/nes.dir/nes/src/cpu.cpp.o


CMakeFiles/nes.dir/nes/src/ines.cpp.o: CMakeFiles/nes.dir/flags.make
CMakeFiles/nes.dir/nes/src/ines.cpp.o: ../nes/src/ines.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oscar/github/nes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/nes.dir/nes/src/ines.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/nes.dir/nes/src/ines.cpp.o -c /home/oscar/github/nes/nes/src/ines.cpp

CMakeFiles/nes.dir/nes/src/ines.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nes.dir/nes/src/ines.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oscar/github/nes/nes/src/ines.cpp > CMakeFiles/nes.dir/nes/src/ines.cpp.i

CMakeFiles/nes.dir/nes/src/ines.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nes.dir/nes/src/ines.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oscar/github/nes/nes/src/ines.cpp -o CMakeFiles/nes.dir/nes/src/ines.cpp.s

CMakeFiles/nes.dir/nes/src/ines.cpp.o.requires:

.PHONY : CMakeFiles/nes.dir/nes/src/ines.cpp.o.requires

CMakeFiles/nes.dir/nes/src/ines.cpp.o.provides: CMakeFiles/nes.dir/nes/src/ines.cpp.o.requires
	$(MAKE) -f CMakeFiles/nes.dir/build.make CMakeFiles/nes.dir/nes/src/ines.cpp.o.provides.build
.PHONY : CMakeFiles/nes.dir/nes/src/ines.cpp.o.provides

CMakeFiles/nes.dir/nes/src/ines.cpp.o.provides.build: CMakeFiles/nes.dir/nes/src/ines.cpp.o


CMakeFiles/nes.dir/nes/src/memory.cpp.o: CMakeFiles/nes.dir/flags.make
CMakeFiles/nes.dir/nes/src/memory.cpp.o: ../nes/src/memory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oscar/github/nes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/nes.dir/nes/src/memory.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/nes.dir/nes/src/memory.cpp.o -c /home/oscar/github/nes/nes/src/memory.cpp

CMakeFiles/nes.dir/nes/src/memory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nes.dir/nes/src/memory.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oscar/github/nes/nes/src/memory.cpp > CMakeFiles/nes.dir/nes/src/memory.cpp.i

CMakeFiles/nes.dir/nes/src/memory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nes.dir/nes/src/memory.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oscar/github/nes/nes/src/memory.cpp -o CMakeFiles/nes.dir/nes/src/memory.cpp.s

CMakeFiles/nes.dir/nes/src/memory.cpp.o.requires:

.PHONY : CMakeFiles/nes.dir/nes/src/memory.cpp.o.requires

CMakeFiles/nes.dir/nes/src/memory.cpp.o.provides: CMakeFiles/nes.dir/nes/src/memory.cpp.o.requires
	$(MAKE) -f CMakeFiles/nes.dir/build.make CMakeFiles/nes.dir/nes/src/memory.cpp.o.provides.build
.PHONY : CMakeFiles/nes.dir/nes/src/memory.cpp.o.provides

CMakeFiles/nes.dir/nes/src/memory.cpp.o.provides.build: CMakeFiles/nes.dir/nes/src/memory.cpp.o


CMakeFiles/nes.dir/nes/src/main.cpp.o: CMakeFiles/nes.dir/flags.make
CMakeFiles/nes.dir/nes/src/main.cpp.o: ../nes/src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oscar/github/nes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/nes.dir/nes/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/nes.dir/nes/src/main.cpp.o -c /home/oscar/github/nes/nes/src/main.cpp

CMakeFiles/nes.dir/nes/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nes.dir/nes/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oscar/github/nes/nes/src/main.cpp > CMakeFiles/nes.dir/nes/src/main.cpp.i

CMakeFiles/nes.dir/nes/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nes.dir/nes/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oscar/github/nes/nes/src/main.cpp -o CMakeFiles/nes.dir/nes/src/main.cpp.s

CMakeFiles/nes.dir/nes/src/main.cpp.o.requires:

.PHONY : CMakeFiles/nes.dir/nes/src/main.cpp.o.requires

CMakeFiles/nes.dir/nes/src/main.cpp.o.provides: CMakeFiles/nes.dir/nes/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/nes.dir/build.make CMakeFiles/nes.dir/nes/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/nes.dir/nes/src/main.cpp.o.provides

CMakeFiles/nes.dir/nes/src/main.cpp.o.provides.build: CMakeFiles/nes.dir/nes/src/main.cpp.o


# Object files for target nes
nes_OBJECTS = \
"CMakeFiles/nes.dir/nes/src/cpu.cpp.o" \
"CMakeFiles/nes.dir/nes/src/ines.cpp.o" \
"CMakeFiles/nes.dir/nes/src/memory.cpp.o" \
"CMakeFiles/nes.dir/nes/src/main.cpp.o"

# External object files for target nes
nes_EXTERNAL_OBJECTS =

nes: CMakeFiles/nes.dir/nes/src/cpu.cpp.o
nes: CMakeFiles/nes.dir/nes/src/ines.cpp.o
nes: CMakeFiles/nes.dir/nes/src/memory.cpp.o
nes: CMakeFiles/nes.dir/nes/src/main.cpp.o
nes: CMakeFiles/nes.dir/build.make
nes: CMakeFiles/nes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/oscar/github/nes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable nes"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/nes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/nes.dir/build: nes

.PHONY : CMakeFiles/nes.dir/build

CMakeFiles/nes.dir/requires: CMakeFiles/nes.dir/nes/src/cpu.cpp.o.requires
CMakeFiles/nes.dir/requires: CMakeFiles/nes.dir/nes/src/ines.cpp.o.requires
CMakeFiles/nes.dir/requires: CMakeFiles/nes.dir/nes/src/memory.cpp.o.requires
CMakeFiles/nes.dir/requires: CMakeFiles/nes.dir/nes/src/main.cpp.o.requires

.PHONY : CMakeFiles/nes.dir/requires

CMakeFiles/nes.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/nes.dir/cmake_clean.cmake
.PHONY : CMakeFiles/nes.dir/clean

CMakeFiles/nes.dir/depend:
	cd /home/oscar/github/nes/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oscar/github/nes /home/oscar/github/nes /home/oscar/github/nes/build /home/oscar/github/nes/build /home/oscar/github/nes/build/CMakeFiles/nes.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/nes.dir/depend
