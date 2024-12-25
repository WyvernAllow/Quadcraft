cmake_minimum_required(VERSION 3.5)
project(quadcraft VERSION 0.1.0 LANGUAGES C)

add_executable(${PROJECT_NAME}
    src/main.c
)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -std=c99 -pedantic -Wall -Wextra)
endif()

add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
)