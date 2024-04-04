if (TARGET polyscope)
  return()
endif()

include(FetchContent)

message(STATUS "Fetching polyscope")

FetchContent_Declare(
    polyscope
    GIT_REPOSITORY https://github.com/nmwsharp/polyscope.git
    GIT_TAG        v2.1.0
    # GIT_TAG        b0913988a29b29f55877a8a9f629fbacc3d73cf8
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(polyscope)

# appends a compiling fix
execute_process(
    COMMAND sed -i "1s/^/#include <cstdint>\\n/" _deps/polyscope-src/include/polyscope/internal.h
    COMMAND_ECHO STDOUT
)

