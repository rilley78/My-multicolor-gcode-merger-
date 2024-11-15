cmake_minimum_required(VERSION 3.10)

project(MulticolorGcodeMerger)

find_package(nfd CONFIG REQUIRED)

add_executable(MulticolorGcodeMerger src/main.cpp)

target_link_libraries(MulticolorGcodeMerger PRIVATE nfd::nfd)