cmake_minimum_required(VERSION 3.20)
project(vkedu-serializer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(example
    examples/example.cpp
)

target_include_directories(example
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()