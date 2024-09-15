cmake_minimum_required(VERSION 3.10)

project(
  cppmagic
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options

option(BUILD_TESTS "Build the tests" ON)
option(ENABLE_TRACEBACK "Enable the traceback" OFF)

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

add_library(cppmagic STATIC ${SOURCES})
target_include_directories(cppmagic PUBLIC include)
target_include_directories(cppmagic PUBLIC thirdparty)

add_executable(cppmagic_cli src/main.cpp)
if(ENABLE_TRACEBACK)
  add_subdirectory(thirdparty/backward-cpp)
  target_link_libraries(cppmagic_cli PRIVATE Backward::Backward)
  target_link_libraries(cppmagic -ldw)
  target_compile_definitions(cppmagic_cli PUBLIC ENABLE_TRACEBACK)
endif()
target_link_libraries(cppmagic_cli PRIVATE cppmagic)

if(BUILD_TESTS)
  enable_testing()

  find_package(GTest REQUIRED)
  include(GoogleTest)

  set(TEST_SOURCES tests/test_FilePath.cpp tests/test_FileSelector.cpp)

  add_executable(cppmagic_test ${TEST_SOURCES})
  target_link_libraries(cppmagic_test PRIVATE cppmagic GTest::GTest GTest::Main)
  gtest_discover_tests(cppmagic_test)
endif()
