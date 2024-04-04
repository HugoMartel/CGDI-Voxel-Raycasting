if (TARGET geometry-central)
  return()
endif()

include(FetchContent)

message(STATUS "Fetching geometry-central")

FetchContent_Declare(
    geometry-central
    GIT_REPOSITORY https://github.com/nmwsharp/geometry-central.git
    GIT_TAG        v0.1.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(geometry-central)

