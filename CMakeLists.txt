cmake_minimum_required(VERSION 2.8.9)
project (blackle-raytracer)

include_directories(src)
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(blackle-raytracer ${SOURCES})
