cmake_minimum_required(VERSION 3.10)
project(3D-Renderer C)

set(CMAKE_C_STANDARD 90)

# Add SDL2
set(SDL2_DIR "C:/SDL2")  # Adjust this path as needed
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Explicitly list header files
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/display.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/triangle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vector.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/light.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/swap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/texture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/array.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/upng.h
)

# Explicitly list source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/display.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/triangle.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vector.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/light.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matrix.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/swap.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/texture.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/array.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/upng.c
)

# Add project source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Create source groups (filters for Visual Studio)
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARIES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)