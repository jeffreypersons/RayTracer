cmake_minimum_required(VERSION 3.15.0)

project(RayTracer VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

add_compile_definitions(
  $<$<CONFIG:DEBUG>:DEBUG>
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googletest)
include(CTest)
include(GoogleTest)

find_package(OpenMP)

add_subdirectory(src)
add_subdirectory(tests)

