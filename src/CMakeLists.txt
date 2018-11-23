find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(Threads REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(
        GLOB_RECURSE
        sources
        ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(app ${sources})

target_link_libraries(app ${CURSES_LIBRARIES})
target_include_directories(app PUBLIC ${CURSES_INCLUDE_DIR})

target_link_libraries (app ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(app PUBLIC ${CMAKE_SOURCE_DIR})

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/app
    DESTINATION bin
    RENAME ${CMAKE_PROJECT_NAME}-app)
