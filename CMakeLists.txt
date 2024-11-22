cmake_minimum_required(VERSION 3.8)

project(Bedlam VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# set opengl config
if (POLICY CMP0072)
  cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

find_package(Git REQUIRED)
if (NOT EXISTS ${LIBTFD_DIR}/CMakeLists.txt)
    execute_process(COMMAND ${GIT_EXECUTABLE}
        submodule update --init --recursive
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()

# add submodule libraries
add_subdirectory("dependencies/la")
add_subdirectory("dependencies/spdlog")
add_subdirectory("dependencies/json")

set(INCLUDE_DIRS
    "include"
)
file(GLOB_RECURSE SRC_FILES "src/**.cpp")
message(STATUS "INCLUDE_DIRS: " ${INCLUDE_DIRS})
message(STATUS "SRC_FILES: ${SRC_FILES}")

# game engine
add_library(marathon ${SRC_FILES})
### TODO: la should be private and considered "external/third-party" to marathon
target_include_directories(marathon PUBLIC ${INCLUDE_DIRS})
target_link_libraries(marathon PUBLIC la)
target_link_libraries(marathon PRIVATE spdlog)
target_link_libraries(marathon PRIVATE nlohmann_json::nlohmann_json)

find_package(OpenGL REQUIRED)
target_include_directories(marathon PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(marathon PUBLIC ${OPENGL_LIBRARIES})
message(STATUS "OpenGL included at ${OPENGL_INCLUDE_DIRS}")
message(STATUS "OpenGL libraries are ${OPENGL_LIBRARIES}")

find_package(SDL2 REQUIRED)
target_include_directories(marathon PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(marathon PUBLIC ${SDL2_LIBRARIES})
message(STATUS "SDL2 included at ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries are ${SDL2_LIBRARIES}")

find_package(GLEW REQUIRED)
target_include_directories(marathon PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(marathon PUBLIC ${GLEW_LIBRARIES})
message(STATUS "GLEW included at ${GLEW_INCLUDE_DIRS}")
message(STATUS "GLEW libraries are ${GLEW_LIBRARIES}")

# runtime
add_executable(example "test/example/src/main.cpp")
target_include_directories(example PUBLIC "test/example/include")
target_link_libraries(example PUBLIC marathon)


add_executable(la_test "test/la_test.cpp")
target_link_libraries(la_test PUBLIC la)
