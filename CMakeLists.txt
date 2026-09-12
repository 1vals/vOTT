cmake_minimum_required(VERSION 3.22)

project(vOTT VERSION 0.0.1)

add_subdirectory(libs/JUCE)

Include (FetchContent)
FetchContent_Declare (melatonin_inspector
        GIT_REPOSITORY https://github.com/sudara/melatonin_inspector.git
        GIT_TAG origin/main
        SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/melatonin_inspector)
FetchContent_MakeAvailable (melatonin_inspector)

add_subdirectory(src)
