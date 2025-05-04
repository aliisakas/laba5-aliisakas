cmake_minimum_required(VERSION 3.10)
project(Laba5)

set(CMAKE_CXX_STANDARD 17)

include_directories(include third_party)

add_executable(blur
    main.cpp
    src/blur.cpp
)
