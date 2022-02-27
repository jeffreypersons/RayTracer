add_executable(RunUnitTests
    Main.cpp
    RayTracer_test.cpp
    Objects_test.cpp
)
target_include_directories(RunUnitTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(RunUnitTests PRIVATE RayTracerCore)
target_link_libraries(RunUnitTests PRIVATE gtest_main)

gtest_discover_tests(RunUnitTests)

