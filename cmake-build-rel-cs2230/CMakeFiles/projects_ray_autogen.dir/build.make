# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Users/sotech117/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/232.9921.42/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/sotech117/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/232.9921.42/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sotech117/Documents/master/graphics/projects-ray-sotech117

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sotech117/Documents/master/graphics/projects-ray-sotech117/cmake-build-rel-cs2230

# Utility rule file for projects_ray_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/projects_ray_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/projects_ray_autogen.dir/progress.make

CMakeFiles/projects_ray_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/sotech117/Documents/master/graphics/projects-ray-sotech117/cmake-build-rel-cs2230/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target projects_ray"
	"/Users/sotech117/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/232.9921.42/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E cmake_autogen /Users/sotech117/Documents/master/graphics/projects-ray-sotech117/cmake-build-rel-cs2230/CMakeFiles/projects_ray_autogen.dir/AutogenInfo.json Release

projects_ray_autogen: CMakeFiles/projects_ray_autogen
projects_ray_autogen: CMakeFiles/projects_ray_autogen.dir/build.make
.PHONY : projects_ray_autogen

# Rule to build all files generated by this target.
CMakeFiles/projects_ray_autogen.dir/build: projects_ray_autogen
.PHONY : CMakeFiles/projects_ray_autogen.dir/build

CMakeFiles/projects_ray_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/projects_ray_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/projects_ray_autogen.dir/clean

CMakeFiles/projects_ray_autogen.dir/depend:
	cd /Users/sotech117/Documents/master/graphics/projects-ray-sotech117/cmake-build-rel-cs2230 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sotech117/Documents/master/graphics/projects-ray-sotech117 /Users/sotech117/Documents/master/graphics/projects-ray-sotech117 /Users/sotech117/Documents/master/graphics/projects-ray-sotech117/cmake-build-rel-cs2230 /Users/sotech117/Documents/master/graphics/projects-ray-sotech117/cmake-build-rel-cs2230 /Users/sotech117/Documents/master/graphics/projects-ray-sotech117/cmake-build-rel-cs2230/CMakeFiles/projects_ray_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/projects_ray_autogen.dir/depend

