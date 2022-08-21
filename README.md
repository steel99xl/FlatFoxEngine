# FlatFoxEngine V0.0.1
A simple fox's game engine in C++

was formerly the KaijuGL Engine

To compile with Conan and CMake:

```bash
# Install conan
pip install conan
# Install all needed dependencies inside a build folder
CONAN_SYSREQUIRES_MODE=enabled conan install . --install-folder build --build missing
# Build
cmake -B build/ # or with ninja: cmake -B build/ -G Ninja
cmake --build build/
cmake --install build/
# Local test
DESTDIR=appdir cmake --install build/
./build/usr/local/bin/LinuxWindow
```
