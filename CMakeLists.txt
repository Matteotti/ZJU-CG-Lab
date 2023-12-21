cmake_minimum_required(VERSION 3.27 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)

project(ZJU_CG_LAB VERSION 0.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJ_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(PROJ_ASSETS_DIR ${PROJ_BINARY_DIR})

set(PROJ_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJ_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# forbids in-source builds
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# debug info
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

add_subdirectory(src)
