cmake_minimum_required(VERSION 3.10)
project(3D-Renderer C)

set(CMAKE_C_STANDARD 90)

# Add SDL2
set(SDL2_DIR "C:/SDL2")  # Adjust this path as needed
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add stb
include_directories(libs/stb)

# Add imgui-docking
include_directories(libs/imgui)

# Add glm
add_subdirectory(libs/glm)

# Add rAudio
include_directories(libs/rAudio/src)

# Add minunit
include_directories(lib/minunit)

# Add project source files
file(GLOB_RECURSE SOURCE_FILES src/*.c)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
    glm
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Add tests
#add_executable(run_tests tests/test_vector.c)
#target_link_libraries(run_tests ${PROJECT_NAME})