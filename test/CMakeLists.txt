cmake_minimum_required(VERSION 3.23)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

message(STATUS "${Yellow}Fetching googletest...${ColorReset}")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0)
FetchContent_GetProperties(googletest)
FetchContent_MakeAvailable(googletest)
message(
  STATUS "${Green}googletest fetched to: ${googletest_SOURCE_DIR}${ColorReset}")

add_executable(sparse_set_test main.cpp)
target_link_libraries(sparse_set_test PRIVATE GTest::gtest_main)
target_include_directories(sparse_set_test PRIVATE ../src)

setup_compiler_flags(sparse_set_test)

add_test(NAME sparse_set_test COMMAND sparse_set_test)
