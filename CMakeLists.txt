cmake_minimum_required(VERSION 3.10)

project(
  cppmagic
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Release build type if none is specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Choose the type of build." FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")

option(BUILD_TESTS "Build the tests" OFF)
option(ENABLE_TRACEBACK "Enable the traceback" ON)

set(SOURCES
    src/CppMagic.cpp
    src/FilePath.cpp
    src/FileSelector.cpp
    src/GenerateMode.cpp
    src/SortMode.cpp
    src/BoomMode.cpp
    src/MagicModeBase.cpp
    src/Settings.cpp
    src/SortingMethod.cpp
    src/TagInfo.cpp
    src/TagManager.cpp
    src/TagUtility.cpp
    src/Utils.cpp)

add_library(cppmagic_lib STATIC ${SOURCES})
target_include_directories(cppmagic_lib PUBLIC include)
target_include_directories(cppmagic_lib PUBLIC thirdparty)

add_executable(cppmagic src/main.cpp)
if(ENABLE_TRACEBACK)
  add_subdirectory(thirdparty/backward-cpp)
  target_link_libraries(cppmagic PRIVATE Backward::Backward)
  target_link_libraries(cppmagic_lib PRIVATE -ldw)
  target_compile_definitions(cppmagic PUBLIC ENABLE_TRACEBACK)
endif()
target_link_libraries(cppmagic PRIVATE cppmagic_lib)

if(BUILD_TESTS)
  enable_testing()

  find_package(GTest REQUIRED)
  include(GoogleTest)

  set(TEST_SOURCES tests/test_FilePath.cpp tests/test_FileSelector.cpp)

  add_executable(cppmagic_test ${TEST_SOURCES})
  target_link_libraries(cppmagic_test PRIVATE cppmagic_lib GTest::GTest
                                              GTest::Main)
  gtest_discover_tests(cppmagic_test)
endif()
