cmake_minimum_required(VERSION 3.19.2)

set(CMAKE_CXX_STANDARD 17)

project(Program)

file(GLOB_RECURSE program_sources CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

add_executable(${PROJECT_NAME} ${program_sources})

link_libraries(stdc++fs pthread)
add_compile_options(--std=c++17)

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)