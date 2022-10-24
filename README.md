# FlatFoxEngine V0.0.1
A simple fox's game engine in C++

was formerly the KaijuGL Engine

To compile with Conan and CMake:

```bash
# Install conan
python3 -m venv
source bin/activate.sh
pip install conan
# Build
cmake -B build/ -DCMAKE_BUILD_TYPE=Debug # or with ninja: cmake -B build/ -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build build/
cmake --install build/
# Local test
DESTDIR=appdir cmake --install build/
# Local installer package
cmake --build build/ --target package
```
