cmake_minimum_required(VERSION 3.20)

project(FlatFoxEngine
    VERSION 0.0.1
    DESCRIPTION "A simple fox's game engine in C++"
)

file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
include(${CMAKE_BINARY_DIR}/conan.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

conan_cmake_configure(
    REQUIRES glew/2.2.0 glfw/3.3.8 glm/cci.20220420
    GENERATORS cmake_find_package
)

conan_cmake_autodetect(settings)

conan_cmake_install(
    PATH_OR_REFERENCE .
    BUILD missing
    SETTINGS ${settings}
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_compile_options(-Wall -pedantic)

set(SOURCES
    main.cpp
    src/FlatFoxRenderWarnings.cpp
    src/Renderer.cpp
    src/FlatFoxWindow.cpp
    src/FrameBufferObject.cpp
    src/SimplePhysics.cpp
    src/Camera.cpp
    src/SimpleObject.cpp
    src/VertexBuffer.cpp
    src/VertexArray.cpp
    src/IndexBuffer.cpp
    src/Shader.cpp
    src/Texture.cpp
    src/Imports/stb_image/stb_image.cpp
    src/Seens/BasicMovement.cpp
)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(LinuxWindow ${SOURCES})

target_include_directories(LinuxWindow PRIVATE
    src/Imports/stb_image
)

target_link_libraries(LinuxWindow PRIVATE
    GLEW::GLEW
    glfw::glfw
    glm::glm
)

install(TARGETS LinuxWindow DESTINATION ${CMAKE_INSTALL_BINDIR})
