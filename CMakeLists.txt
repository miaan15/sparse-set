cmake_minimum_required(VERSION 3.23)

project(sparse_set LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/compiler-flags.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/message-color.cmake")

if(BUILD_TEST)
  message(STATUS "${Yellow}Build test${ColorReset}")
  enable_testing()
  add_subdirectory(test)
endif()

if(BUILD_BENCH)
  message(STATUS "${Yellow}Build bench${ColorReset}")
  add_subdirectory(bench)
endif()
