cmake_minimum_required(VERSION 3.20)

include(FetchContent)

find_package(GTest QUIET)

if (NOT GTest_FOUND)
  message(STATUS "GTest not found, fetching googletest...")
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

file(GLOB_RECURSE REGISTRY_TEST_SOURCES
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/integration/*.cpp"
)

add_executable(registry_tests ${REGISTRY_TEST_SOURCES})

target_link_libraries(registry_tests
  PRIVATE
    registry_core
    GTest::gtest
    GTest::gtest_main
)

if (MSVC)
  target_compile_options(registry_tests PRIVATE /W4 /permissive-)
else()
  target_compile_options(registry_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(GoogleTest)
gtest_discover_tests(registry_tests
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  DISCOVERY_MODE PRE_TEST
)
