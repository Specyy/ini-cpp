# CMake 3.14 required to build (could maybe set lower?)
cmake_minimum_required(VERSION 3.14)

# Set the project with latest version
project(ini-cpp)

# List of source files for the project
set(SOURCE_FILES 
    src/main.cpp
)

# Set the executable file for the project (should change to lib later)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Add includes for the project
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Require c++17 to compile the project
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Enable lto on the target if supported (in Release mode)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()

# Enable PIE/PIC on the target if supported
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE True)