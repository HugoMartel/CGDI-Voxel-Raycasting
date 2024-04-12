CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

project(CGDI-Voxel-Raycasting VERSION 1.0)

# Check build type
if (NOT CMAKE_BUILD_TYPE)
    message("Build Type not set, defaulting to Debug..." )
    message(" Use \"-DCMAKE_BUILD_TYPE=Release\" if you would like the Release build")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiling options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_BUILD_TYPE EQUAL Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O1 -g")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3")
endif()

# Load scripts to automatically fetch dependencies
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Include needed header files
include_directories(${PROJECT_SOURCE_DIR}/deps)

############################################################
# DEPS
include(polyscope)
find_package(jsoncpp REQUIRED)

############################################################
# Executable
add_executable(raycast)
set_target_properties(raycast PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-pragmas -Wno-missing-field-initializers")
target_include_directories(raycast PUBLIC include)
target_sources(raycast PUBLIC src/main.cpp src/argparser.cpp src/voxel.cpp src/scene.cpp src/util.cpp)
target_link_libraries(raycast PUBLIC polyscope jsoncpp_lib)

