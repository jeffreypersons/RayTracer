cmake_minimum_required(VERSION 3.15.0)

project(RayTracer VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(src)

