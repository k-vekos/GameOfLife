# CMakeList.txt : CMake project for GameOfLife, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Give the project a name
project(GameOfLife)

# Include SFML
set(SFML_STATIC_LIBRARIES TRUE)
#set(SFML_ROOT C:/SFML-2.5.1)
set (CMAKE_PREFIX_PATH "C:\\SFML-2.5.1\\lib\\cmake")
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

# Add source to this project's executable.
#add_executable (GameOfLife "src/GameOfLife.cpp" "src/GameOfLife.h")

set(SOURCES 
	src/Main.cpp
    src/GameOfLife.cpp
	src/GameOfLife.h
)

add_executable(GameOfLife ${SOURCES})

# Link SFML
target_link_libraries(GameOfLife sfml-graphics)

# TODO: Add tests and install targets if needed.
