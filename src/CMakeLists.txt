find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
    main.cpp
    Website/website.cpp
    Website/website.hpp
    Pinger/pinger.cpp
    Pinger/pinger.hpp
)

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/main
    DESTINATION bin
    RENAME ${CMAKE_PROJECT_NAME}-main)
