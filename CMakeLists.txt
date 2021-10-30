cmake_minimum_required(VERSION 3.12)
project(OpenGL VERSION 0.1.0)

file(GLOB HEADERS "src/*.h")
file(GLOB SRC "src/*.cpp")
# src/FlyCamera.cpp src/RenderInfo.cp
add_executable(OpenGL src/main.cpp ${SRC} ${HEADERS})

target_include_directories(OpenGL PRIVATE deps/glm
                                          deps/stb
                                          deps/glfw/include
                                          deps/glew/include)

target_link_directories(OpenGL PRIVATE deps/glfw/lib-vc2019
                                       deps/glew/lib/Release/x64)

target_link_libraries(OpenGL PRIVATE glfw3.lib
                                     glew32.lib
                                     opengl32.lib)

message("${CMAKE_BUILD_TYPE}")

add_custom_command(TARGET OpenGL POST_BUILD 
                COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/glew/bin/Release/x64/glew32.dll" "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/glew32.dll"
                COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaders"
                COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/textures" "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/textures"
)

