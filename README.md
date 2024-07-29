# SDL 3D Renderer

This is a C-based game project using SDL2, ImGui, GLM, rAudio, and other libraries. The project is set up to be developed using Visual Studio 2022 and CMake.

## Prerequisites

- Visual Studio 2022
- CMake (3.10 or higher)
- Git

## Setup Instructions

1. Clone the repository:
   ```
   git clone https://github.com/ramo1606/SDL-3D-Renderer.git
   cd my-game-project
   ```

2. Install SDL2:
   - Download SDL2 development libraries for Windows from https://github.com/libsdl-org/SDL/releases/latest
   - Extract the ZIP file to a location on your computer (e.g., C:\SDL2)
   - Add the SDL2 bin directory (e.g., C:\SDL2\lib\x64) to your system's PATH environment variable
   - Create a sdl2-config.cmake and add the following:
        ```cmake
        set(SDL2_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")

        # Support both 32 and 64 bit builds
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2main.lib")
        else ()
            set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2main.lib")
        endif ()

        string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
        ```

3. Install other dependencies:
   ```
   git submodule add https://github.com/ocornut/imgui.git lib/imgui
   git submodule add https://github.com/g-truc/glm.git lib/glm
   git submodule add https://github.com/raysan5/raudio.git lib/rAudio
   ```

4. Download stb_image.h and stb_truetype.h:
   - Create a `libs/stb` directory
   - Download [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) and [stb_truetype.h](https://github.com/nothings/stb/blob/master/stb_truetype.h) to this directory

5. Download minunit.h:
   - Create a `libs/minunit` directory
   - Download [minunit.h](https://github.com/siu/minunit/blob/master/minunit.h) to this directory

6. Generate Visual Studio project files:
   ```
   mkdir build
   cd build
   cmake -G "Visual Studio 17 2022" -A x64 ..
   ```

7. Open the generated .sln file in Visual Studio 2022

8. In Visual Studio, set the startup project to "MyGameProject" (right-click on MyGameProject in Solution Explorer and select "Set as Startup Project")

9. Build and run the project (F5 or Debug > Start Debugging)

## Project Structure

- `assets/`: Contains assets files
- `docs/`: Contains documentation
- `src/`: Contains main source files (main.c, game.c)
- `include/`: Contains header files (game.h)
- `libs/`: External libraries
    - `imgui/`: ImGui library (submodule)
    - `glm/`: GLM library (submodule)
    - `rAudio/`: rAudio library (submodule)
    - `stb/`: stb_image.h and stb_truetype.h
    - `minunit/`: minunit.h
- `tests/`: Test files using minunit
- `CMakeLists.txt`: CMake configuration file
- `README.md`: This file

## Development Workflow

1. Write your game logic in `src/game.c` and `include/game.h`
2. Add new source files to `CMakeLists.txt` if needed
3. Use ImGui for UI, GLM for math operations, and rAudio for sound
4. Write tests in `tests/test_main.c`
5. Commit changes regularly:
   ```
   git add .
   git commit -m "Description of changes"
   git push origin main
   ```

## Troubleshooting

- If SDL2 is not found, ensure the SDL2 directory is in your PATH and that CMake can find it
- If you encounter linker errors, check that all libraries are correctly linked in CMakeLists.txt
- For any other issues, refer to the documentation of the respective libraries or seek help in their community forums

## License

[Your chosen license]