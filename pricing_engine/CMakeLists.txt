cmake_minimum_required(VERSION 3.15)
project(myproject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(pricing_engine_lib
    src/DataLoader.cpp
)
target_include_directories(pricing_engine_lib PUBLIC include)

enable_testing()
add_subdirectory(tests)