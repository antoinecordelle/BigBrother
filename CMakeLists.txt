cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release)

project(BigBrother)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(doc)

enable_testing ()
add_test (NAME Test
          COMMAND tests)
