cmake_minimum_required(VERSION 3.16)
project(GameOfLife LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML REQUIRED COMPONENTS Graphics Window System)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/initial_configs.cpp
)

target_link_libraries(${PROJECT_NAME} SFML::Graphics SFML::Window SFML::System)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/res/JetBrainsMonoNerdFont-Light.ttf
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
