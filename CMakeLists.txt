cmake_minimum_required(VERSION 3.21)
project(1uloha C)

set(CMAKE_C_STANDARD 11)

include_directories(c202)
include_directories(c204)
include_directories(c206)

add_executable(1uloha
        c202/c202-advanced-test.c
        c202/c202.c
        c202/c202.h
        c204/c204-test.c
        c204/c204.c
        c204/c204.h
        c206/c206-advanced-test.c
        c206/c206.c
        c206/c206.h
        )
