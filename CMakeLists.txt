cmake_minimum_required(VERSION 3.29)
project(LinearAlgebraLib)

set(SRC_MATH Sources/Math/Matrix.cpp)

add_subdirectory(cunit/CUnit)

include_directories(cunit/CUnit Sources)

add_executable(LinearAlgebraLib
    Sources/Application.cpp
    ${SRC_MATH}
)

target_link_libraries(LinearAlgebraLib PRIVATE cunit)