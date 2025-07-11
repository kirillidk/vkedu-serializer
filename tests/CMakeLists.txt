include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(serializer_tests serializer.cpp)

target_include_directories(serializer_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(serializer_tests
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(serializer_tests)