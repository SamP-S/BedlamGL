cmake_minimum_required(VERSION 3.8)

project(LinearAlgebruh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# build extended LA library for 3D/OpenGL usage
set(SRC
	src/assertfunc.cpp
	src/la_extended.cpp
)
add_library(la STATIC ${SRC})
target_include_directories(la PUBLIC include)
target_include_directories(la PUBLIC .)
