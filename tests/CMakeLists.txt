set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/tests/cmake/)

include(FetchContent)
include(CodeCoverage)
include(GoogleTest)

enable_testing()

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)

FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)

target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(tests
    predicate.cpp
    interval_algebra.cpp
    test_obstable_fill.cpp
    test_word.cpp)

target_link_libraries(tests
    PUBLIC
        GTest::GTest)
        #symaut)

target_include_directories(tests PRIVATE ../include/)

gtest_discover_tests(tests)

setup_target_for_coverage(coverage tests coverage)

