# CMake 3.14 required to build (could maybe set lower?)
cmake_minimum_required(VERSION 3.14)

# Set the project
project(ini-cpp)

# Whether or not to build tests
option(INICPP_TEST "Builds inicpp library tests" ON)

# List of source files for the project
set(SOURCE_FILES 
    src/ini_value.cpp
    src/ini_section.cpp
    src/ini.cpp
    src/parser_exception.cpp
)

# Set the executable file for the project (should change to lib later)
add_library("${PROJECT_NAME}" SHARED ${SOURCE_FILES})
add_library("${PROJECT_NAME}-static" STATIC ${SOURCE_FILES})

# Add macro indicating source should be built
target_compile_definitions("${PROJECT_NAME}" PRIVATE INICPP_BUILD=1)
target_compile_definitions("${PROJECT_NAME}" PRIVATE INICPP_DLL=1)
target_compile_definitions("${PROJECT_NAME}-static" PRIVATE INICPP_BUILD=1)

# Add includes for the project
target_include_directories("${PROJECT_NAME}" PRIVATE src include/ini-cpp)
target_include_directories("${PROJECT_NAME}-static" PRIVATE src include/ini-cpp)

# Require c++17 to compile the project
target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_17)
target_compile_features("${PROJECT_NAME}-static" PRIVATE cxx_std_17)

# Enable lto on the target if supported (in Release mode)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET "${PROJECT_NAME}" PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
    set_property(TARGET "${PROJECT_NAME}-static" PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()

# Enable PIE/PIC on the target if supported
set_property(TARGET "${PROJECT_NAME}" PROPERTY POSITION_INDEPENDENT_CODE True)

# Build tests if set to do so
if(INICPP_TEST)
    # Test executable name
    set(INICPP_TEST_NAME ini-cpp-test)

    # Test target
    add_executable(${INICPP_TEST_NAME} test/src/main.cpp)

    # Test target properties - global
    target_include_directories(${INICPP_TEST_NAME} PRIVATE include test/src)
    target_compile_features(${INICPP_TEST_NAME} PRIVATE cxx_std_17)

    # Test target properties - uses shared library
    target_link_libraries(${INICPP_TEST_NAME} PRIVATE "${PROJECT_NAME}")
    target_compile_definitions(${INICPP_TEST_NAME} PRIVATE INICPP_DLL=1)

    # Test target properties - uses static library
    # target_link_libraries(${INICPP_TEST_NAME} PRIVATE "${PROJECT_NAME}-static")
endif()

