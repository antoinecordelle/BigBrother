find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

include_directories (${CMAKE_SOURCE_DIR}/src/)

file(
        GLOB_RECURSE
        sources
        ${CMAKE_SOURCE_DIR}/src/*.cpp
)
list(REMOVE_ITEM sources ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable (runTests ${sources} test.cpp)

target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
