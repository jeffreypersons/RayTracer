add_library(RayTracerCore
    Camera.cpp
    FrameBuffer.cpp
    Lights.cpp
    Material.cpp
    Objects.cpp
    RayTracer.cpp
    Scene.cpp
    StopWatch.cpp
)
target_include_directories(RayTracerCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(OpenMP_CXX_FOUND)
    target_link_libraries(RayTracerCore PUBLIC OpenMP::OpenMP_CXX)
endif()


add_executable(TraceScene
    Main.cpp
    App.cpp
)
target_link_libraries(TraceScene PRIVATE RayTracerCore)

install(TARGETS TraceScene RUNTIME)

