CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

project(CGDI-Voxel-Raycasting VERSION 1.0)

# Check build type
if (NOT CMAKE_BUILD_TYPE)
    message("Build Type not set, defaulting to Debug..." )
    message(" Use \"-DCMAKE_BUILD_TYPE=Release\" if you would like the Release build")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiling options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (CMAKE_BUILD_TYPE EQUAL Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
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
target_sources(raycast PUBLIC src/main.cpp
                              src/argparser.cpp
                              src/geometry.cpp
                              src/voxel.cpp
                              src/scene.cpp
                              src/ray.cpp
                              src/ray_algorithm.cpp
                              src/util.cpp)
target_link_libraries(raycast PUBLIC polyscope jsoncpp_lib)

