cmake_minimum_required(VERSION 3.20)

project(FlatFoxEngine
    VERSION 1.0
    DESCRIPTION "A simple fox's game engine in C++"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_compile_options(-Wall -pedantic)

set(SOURCES
    main.cpp src/FlatFoxRenderWarnings.cpp src/Renderer.cpp src/FlatFoxWindow.cpp src/FrameBufferObject.cpp src/SimplePhysics.cpp src/Camera.cpp src/SimpleObject.cpp src/VertexBuffer.cpp src/VertexArray.cpp src/IndexBuffer.cpp src/Shader.cpp src/Texture.cpp src/Imports/stb_image/stb_image.cpp  src/Seens/BasicMovement.cpp
)

add_executable(LinuxWindow ${SOURCES})

target_link_libraries(LinuxWindow GLEW GL glfw)
