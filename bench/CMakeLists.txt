cmake_minimum_required(VERSION 3.23)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

message(STATUS "${Yellow}Fetching benchmark...${ColorReset}")
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.4)
FetchContent_GetProperties(benchmark)
FetchContent_MakeAvailable(benchmark)
message(
  STATUS "${Green}benchmark fetched to: ${benchmark_SOURCE_DIR}${ColorReset}")

add_executable(sparse_set_bench main.cpp)
target_link_libraries(sparse_set_bench PRIVATE benchmark::benchmark)
target_include_directories(sparse_set_bench PRIVATE ../src)
