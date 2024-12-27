# Quadcraft
A Minecraft clone in C

# Building
## Prequisites
* [git](https://git-scm.com/)
* [CMake](https://cmake.org/)
* Any C99 compatible C compiler (GCC, Clang, MSVC, etc.)

## Build steps
```shell
# Clone the repository and all submodules
git clone --recursive https://github.com/WyvernAllow/quadcraft.git
cd quadcraft

# Configure with CMake
cmake -S . -B build

# Build the project
cmake --build build --config Release
```

*Depending on the platform or build tool used, the `quadcraft.exe` file will be located somewhere in the `build` directory. Ensure that the `res` directory is located in the working directory of the executable.*

# Libraries used
* [glad](https://gen.glad.sh/)
* [glfw](https://www.glfw.org/)
* [stb_image](https://github.com/nothings/stb)

# License
Quadcraft is distributed under the zlib license, available in the `LICENSE.txt` file.