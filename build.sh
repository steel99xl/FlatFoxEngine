#/bin/sh
clear
echo "Building App..."
clang++ -Wall -pedantic -o LinuxWindow -std=c++20 main.cpp src/FlatFoxRenderWarnings.cpp src/Renderer.cpp src/FlatFoxWindow.cpp src/FrameBufferObject.cpp src/SimplePhysics.cpp src/Camera.cpp src/SimpleObject.cpp src/VertexBuffer.cpp src/VertexArray.cpp src/IndexBuffer.cpp src/Shader.cpp src/Texture.cpp src/Imports/stb_image/stb_image.cpp  src/Seens/BasicMovement.cpp -L src/Imports/glfw/build/src/ -lglfw3 -L src/Imports/glew/lib/ -lGLEW
