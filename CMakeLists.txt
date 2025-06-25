cmake_minimum_required(VERSION 3.18)
project(ACO_SFML LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE CUDA_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cu"
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

add_executable(sfml_game
    ${CPP_SOURCES}
    ${CUDA_SOURCES}
)

target_link_libraries(sfml_game
    sfml-graphics
    sfml-window
    sfml-system
)

add_custom_command(TARGET sfml_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:sfml_game>/assets
)
