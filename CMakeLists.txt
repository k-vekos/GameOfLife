# CMakeList.txt : CMake project for GameOfLife
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Give the project a name
project(GameOfLife)

# Include SFML
set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_PREFIX_PATH "C:\\SFML-2.5.1\\lib\\cmake;D:\\SFML-2.5.1\\lib\\cmake")
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)


# Include OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(SOURCES 
	src/Main.cpp
    src/GameOfLife.cpp
	src/GameOfLife.h
	src/WorldRenderer.cpp
	src/WorldRenderer.h
	src/Cell.h
)

add_executable(GameOfLife ${SOURCES})

target_link_libraries(GameOfLife PUBLIC OpenMP::OpenMP_CXX sfml-graphics)

if(MSVC)
  target_compile_options(GameOfLife PRIVATE /W4 /WX)
else(MSVC)
  target_compile_options(GameOfLife PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)        "/WX /W4">)