cmake_minimum_required(VERSION 3.14)

project(SymbolicAutomata)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter")
# set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-sign-compare")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O0 -pg -fprofile-instr-generate -fcoverage-mapping ${CMAKE_CXX_FLAGS_DEBUG}")

# set(SOURCES 
    # src/
    # src/Oracle.cpp)

# add_library(symaut SHARED ${SOURCES})

# target_include_directories(symaut PRIVATE include/)

add_subdirectory(tests)
