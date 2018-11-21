find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(
        GLOB_RECURSE
        sources
        ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CURSES_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/main
    DESTINATION bin
    RENAME ${CMAKE_PROJECT_NAME}-main)
