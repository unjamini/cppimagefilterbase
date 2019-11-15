cmake_minimum_required(VERSION 2.8)

project(student_toolkit)
add_executable(${PROJECT_NAME} "main.cpp" "png_toolkit.cpp" "png_toolkit.h"
    "stb_image.h" "stb_image_write.h" filterSeq.cpp filterSeq.h FilterUnit.cpp FilterUnit.h)

set(CMAKE_CXX_STANDARD 14)
