# CMAKE generated file: DO NOT EDIT!
# Generated by "MSYS Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = "/C/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "/C/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /C/Dev/RenderEngine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /C/Dev/RenderEngine/Temp

# Include any dependencies generated for this target.
include CMakeFiles/Engine.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Engine.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Engine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Engine.dir/flags.make

CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.obj: CMakeFiles/Engine.dir/includes_C.rsp
CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.obj: C:/Dev/RenderEngine/Source/3rdParty/glad/glad.c
CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.obj"
	/C/MSYS64/ucrt64/bin/cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.obj -MF CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.obj.d -o CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.obj -c /C/Dev/RenderEngine/Source/3rdParty/glad/glad.c

CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.i"
	/C/MSYS64/ucrt64/bin/cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /C/Dev/RenderEngine/Source/3rdParty/glad/glad.c > CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.i

CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.s"
	/C/MSYS64/ucrt64/bin/cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /C/Dev/RenderEngine/Source/3rdParty/glad/glad.c -o CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.s

CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.obj: C:/Dev/RenderEngine/Source/3rdParty/glm/detail/glm.cpp
CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.obj -MF CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.obj.d -o CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.obj -c /C/Dev/RenderEngine/Source/3rdParty/glm/detail/glm.cpp

CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/3rdParty/glm/detail/glm.cpp > CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.i

CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/3rdParty/glm/detail/glm.cpp -o CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.s

CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.obj: CMakeFiles/Engine.dir/includes_C.rsp
CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.obj: C:/Dev/RenderEngine/Source/3rdParty/stb/stb_image.c
CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.obj"
	/C/MSYS64/ucrt64/bin/cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.obj -MF CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.obj.d -o CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.obj -c /C/Dev/RenderEngine/Source/3rdParty/stb/stb_image.c

CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.i"
	/C/MSYS64/ucrt64/bin/cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /C/Dev/RenderEngine/Source/3rdParty/stb/stb_image.c > CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.i

CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.s"
	/C/MSYS64/ucrt64/bin/cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /C/Dev/RenderEngine/Source/3rdParty/stb/stb_image.c -o CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.s

CMakeFiles/Engine.dir/Source/Application.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/Application.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/Application.cpp.obj: C:/Dev/RenderEngine/Source/Application.cpp
CMakeFiles/Engine.dir/Source/Application.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Engine.dir/Source/Application.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/Application.cpp.obj -MF CMakeFiles/Engine.dir/Source/Application.cpp.obj.d -o CMakeFiles/Engine.dir/Source/Application.cpp.obj -c /C/Dev/RenderEngine/Source/Application.cpp

CMakeFiles/Engine.dir/Source/Application.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/Application.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/Application.cpp > CMakeFiles/Engine.dir/Source/Application.cpp.i

CMakeFiles/Engine.dir/Source/Application.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/Application.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/Application.cpp -o CMakeFiles/Engine.dir/Source/Application.cpp.s

CMakeFiles/Engine.dir/Source/Buffers.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/Buffers.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/Buffers.cpp.obj: C:/Dev/RenderEngine/Source/Buffers.cpp
CMakeFiles/Engine.dir/Source/Buffers.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Engine.dir/Source/Buffers.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/Buffers.cpp.obj -MF CMakeFiles/Engine.dir/Source/Buffers.cpp.obj.d -o CMakeFiles/Engine.dir/Source/Buffers.cpp.obj -c /C/Dev/RenderEngine/Source/Buffers.cpp

CMakeFiles/Engine.dir/Source/Buffers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/Buffers.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/Buffers.cpp > CMakeFiles/Engine.dir/Source/Buffers.cpp.i

CMakeFiles/Engine.dir/Source/Buffers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/Buffers.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/Buffers.cpp -o CMakeFiles/Engine.dir/Source/Buffers.cpp.s

CMakeFiles/Engine.dir/Source/Lights.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/Lights.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/Lights.cpp.obj: C:/Dev/RenderEngine/Source/Lights.cpp
CMakeFiles/Engine.dir/Source/Lights.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Engine.dir/Source/Lights.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/Lights.cpp.obj -MF CMakeFiles/Engine.dir/Source/Lights.cpp.obj.d -o CMakeFiles/Engine.dir/Source/Lights.cpp.obj -c /C/Dev/RenderEngine/Source/Lights.cpp

CMakeFiles/Engine.dir/Source/Lights.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/Lights.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/Lights.cpp > CMakeFiles/Engine.dir/Source/Lights.cpp.i

CMakeFiles/Engine.dir/Source/Lights.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/Lights.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/Lights.cpp -o CMakeFiles/Engine.dir/Source/Lights.cpp.s

CMakeFiles/Engine.dir/Source/Material.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/Material.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/Material.cpp.obj: C:/Dev/RenderEngine/Source/Material.cpp
CMakeFiles/Engine.dir/Source/Material.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Engine.dir/Source/Material.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/Material.cpp.obj -MF CMakeFiles/Engine.dir/Source/Material.cpp.obj.d -o CMakeFiles/Engine.dir/Source/Material.cpp.obj -c /C/Dev/RenderEngine/Source/Material.cpp

CMakeFiles/Engine.dir/Source/Material.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/Material.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/Material.cpp > CMakeFiles/Engine.dir/Source/Material.cpp.i

CMakeFiles/Engine.dir/Source/Material.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/Material.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/Material.cpp -o CMakeFiles/Engine.dir/Source/Material.cpp.s

CMakeFiles/Engine.dir/Source/Mesh.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/Mesh.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/Mesh.cpp.obj: C:/Dev/RenderEngine/Source/Mesh.cpp
CMakeFiles/Engine.dir/Source/Mesh.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Engine.dir/Source/Mesh.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/Mesh.cpp.obj -MF CMakeFiles/Engine.dir/Source/Mesh.cpp.obj.d -o CMakeFiles/Engine.dir/Source/Mesh.cpp.obj -c /C/Dev/RenderEngine/Source/Mesh.cpp

CMakeFiles/Engine.dir/Source/Mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/Mesh.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/Mesh.cpp > CMakeFiles/Engine.dir/Source/Mesh.cpp.i

CMakeFiles/Engine.dir/Source/Mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/Mesh.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/Mesh.cpp -o CMakeFiles/Engine.dir/Source/Mesh.cpp.s

CMakeFiles/Engine.dir/Source/Scene.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/Scene.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/Scene.cpp.obj: C:/Dev/RenderEngine/Source/Scene.cpp
CMakeFiles/Engine.dir/Source/Scene.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Engine.dir/Source/Scene.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/Scene.cpp.obj -MF CMakeFiles/Engine.dir/Source/Scene.cpp.obj.d -o CMakeFiles/Engine.dir/Source/Scene.cpp.obj -c /C/Dev/RenderEngine/Source/Scene.cpp

CMakeFiles/Engine.dir/Source/Scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/Scene.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/Scene.cpp > CMakeFiles/Engine.dir/Source/Scene.cpp.i

CMakeFiles/Engine.dir/Source/Scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/Scene.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/Scene.cpp -o CMakeFiles/Engine.dir/Source/Scene.cpp.s

CMakeFiles/Engine.dir/Source/SceneNodes.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/SceneNodes.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/SceneNodes.cpp.obj: C:/Dev/RenderEngine/Source/SceneNodes.cpp
CMakeFiles/Engine.dir/Source/SceneNodes.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Engine.dir/Source/SceneNodes.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/SceneNodes.cpp.obj -MF CMakeFiles/Engine.dir/Source/SceneNodes.cpp.obj.d -o CMakeFiles/Engine.dir/Source/SceneNodes.cpp.obj -c /C/Dev/RenderEngine/Source/SceneNodes.cpp

CMakeFiles/Engine.dir/Source/SceneNodes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/SceneNodes.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/SceneNodes.cpp > CMakeFiles/Engine.dir/Source/SceneNodes.cpp.i

CMakeFiles/Engine.dir/Source/SceneNodes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/SceneNodes.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/SceneNodes.cpp -o CMakeFiles/Engine.dir/Source/SceneNodes.cpp.s

CMakeFiles/Engine.dir/Source/Shader.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/Shader.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/Shader.cpp.obj: C:/Dev/RenderEngine/Source/Shader.cpp
CMakeFiles/Engine.dir/Source/Shader.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Engine.dir/Source/Shader.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/Shader.cpp.obj -MF CMakeFiles/Engine.dir/Source/Shader.cpp.obj.d -o CMakeFiles/Engine.dir/Source/Shader.cpp.obj -c /C/Dev/RenderEngine/Source/Shader.cpp

CMakeFiles/Engine.dir/Source/Shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/Shader.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/Shader.cpp > CMakeFiles/Engine.dir/Source/Shader.cpp.i

CMakeFiles/Engine.dir/Source/Shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/Shader.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/Shader.cpp -o CMakeFiles/Engine.dir/Source/Shader.cpp.s

CMakeFiles/Engine.dir/Source/Texture.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/Texture.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/Texture.cpp.obj: C:/Dev/RenderEngine/Source/Texture.cpp
CMakeFiles/Engine.dir/Source/Texture.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/Engine.dir/Source/Texture.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/Texture.cpp.obj -MF CMakeFiles/Engine.dir/Source/Texture.cpp.obj.d -o CMakeFiles/Engine.dir/Source/Texture.cpp.obj -c /C/Dev/RenderEngine/Source/Texture.cpp

CMakeFiles/Engine.dir/Source/Texture.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/Texture.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/Texture.cpp > CMakeFiles/Engine.dir/Source/Texture.cpp.i

CMakeFiles/Engine.dir/Source/Texture.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/Texture.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/Texture.cpp -o CMakeFiles/Engine.dir/Source/Texture.cpp.s

CMakeFiles/Engine.dir/Source/VertexArray.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/VertexArray.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/VertexArray.cpp.obj: C:/Dev/RenderEngine/Source/VertexArray.cpp
CMakeFiles/Engine.dir/Source/VertexArray.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/Engine.dir/Source/VertexArray.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/VertexArray.cpp.obj -MF CMakeFiles/Engine.dir/Source/VertexArray.cpp.obj.d -o CMakeFiles/Engine.dir/Source/VertexArray.cpp.obj -c /C/Dev/RenderEngine/Source/VertexArray.cpp

CMakeFiles/Engine.dir/Source/VertexArray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/VertexArray.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/VertexArray.cpp > CMakeFiles/Engine.dir/Source/VertexArray.cpp.i

CMakeFiles/Engine.dir/Source/VertexArray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/VertexArray.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/VertexArray.cpp -o CMakeFiles/Engine.dir/Source/VertexArray.cpp.s

CMakeFiles/Engine.dir/Source/main.cpp.obj: CMakeFiles/Engine.dir/flags.make
CMakeFiles/Engine.dir/Source/main.cpp.obj: CMakeFiles/Engine.dir/includes_CXX.rsp
CMakeFiles/Engine.dir/Source/main.cpp.obj: C:/Dev/RenderEngine/Source/main.cpp
CMakeFiles/Engine.dir/Source/main.cpp.obj: CMakeFiles/Engine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/Engine.dir/Source/main.cpp.obj"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Engine.dir/Source/main.cpp.obj -MF CMakeFiles/Engine.dir/Source/main.cpp.obj.d -o CMakeFiles/Engine.dir/Source/main.cpp.obj -c /C/Dev/RenderEngine/Source/main.cpp

CMakeFiles/Engine.dir/Source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Engine.dir/Source/main.cpp.i"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /C/Dev/RenderEngine/Source/main.cpp > CMakeFiles/Engine.dir/Source/main.cpp.i

CMakeFiles/Engine.dir/Source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Engine.dir/Source/main.cpp.s"
	/C/MSYS64/ucrt64/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /C/Dev/RenderEngine/Source/main.cpp -o CMakeFiles/Engine.dir/Source/main.cpp.s

# Object files for target Engine
Engine_OBJECTS = \
"CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.obj" \
"CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.obj" \
"CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.obj" \
"CMakeFiles/Engine.dir/Source/Application.cpp.obj" \
"CMakeFiles/Engine.dir/Source/Buffers.cpp.obj" \
"CMakeFiles/Engine.dir/Source/Lights.cpp.obj" \
"CMakeFiles/Engine.dir/Source/Material.cpp.obj" \
"CMakeFiles/Engine.dir/Source/Mesh.cpp.obj" \
"CMakeFiles/Engine.dir/Source/Scene.cpp.obj" \
"CMakeFiles/Engine.dir/Source/SceneNodes.cpp.obj" \
"CMakeFiles/Engine.dir/Source/Shader.cpp.obj" \
"CMakeFiles/Engine.dir/Source/Texture.cpp.obj" \
"CMakeFiles/Engine.dir/Source/VertexArray.cpp.obj" \
"CMakeFiles/Engine.dir/Source/main.cpp.obj"

# External object files for target Engine
Engine_EXTERNAL_OBJECTS =

Engine.exe: CMakeFiles/Engine.dir/Source/3rdParty/glad/glad.c.obj
Engine.exe: CMakeFiles/Engine.dir/Source/3rdParty/glm/detail/glm.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/3rdParty/stb/stb_image.c.obj
Engine.exe: CMakeFiles/Engine.dir/Source/Application.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/Buffers.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/Lights.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/Material.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/Mesh.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/Scene.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/SceneNodes.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/Shader.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/Texture.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/VertexArray.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/Source/main.cpp.obj
Engine.exe: CMakeFiles/Engine.dir/build.make
Engine.exe: C:/MSYS64/ucrt64/lib/libmingw32.a
Engine.exe: C:/Dev/RenderEngine/Lib/libSDL2main.a
Engine.exe: C:/Dev/RenderEngine/Lib/libSDL2.dll.a
Engine.exe: C:/MSYS64/ucrt64/lib/libopengl32.a
Engine.exe: CMakeFiles/Engine.dir/linkLibs.rsp
Engine.exe: CMakeFiles/Engine.dir/objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/C/Dev/RenderEngine/Temp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking CXX executable Engine.exe"
	"/C/Program Files/CMake/bin/cmake.exe" -E rm -f CMakeFiles/Engine.dir/objects.a
	/C/MSYS64/ucrt64/bin/ar.exe qc CMakeFiles/Engine.dir/objects.a @CMakeFiles/Engine.dir/objects1.rsp
	/C/MSYS64/ucrt64/bin/c++.exe -Wl,--whole-archive CMakeFiles/Engine.dir/objects.a -Wl,--no-whole-archive -o Engine.exe -Wl,--out-implib,libEngine.dll.a -Wl,--major-image-version,0,--minor-image-version,0 @CMakeFiles/Engine.dir/linkLibs.rsp

# Rule to build all files generated by this target.
CMakeFiles/Engine.dir/build: Engine.exe
.PHONY : CMakeFiles/Engine.dir/build

CMakeFiles/Engine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Engine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Engine.dir/clean

CMakeFiles/Engine.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MSYS Makefiles" /C/Dev/RenderEngine /C/Dev/RenderEngine /C/Dev/RenderEngine/Temp /C/Dev/RenderEngine/Temp /C/Dev/RenderEngine/Temp/CMakeFiles/Engine.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Engine.dir/depend

