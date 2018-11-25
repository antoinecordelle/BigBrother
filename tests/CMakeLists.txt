find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(Threads REQUIRED)
include_directories(${THREADS_INCLUDE_DIR})

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

target_link_libraries(runTests ${CURSES_LIBRARIES})
target_link_libraries (runTests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

target_include_directories(runTests PUBLIC ${CURSES_INCLUDE_DIR})
