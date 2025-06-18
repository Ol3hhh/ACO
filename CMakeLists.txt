cmake_minimum_required(VERSION 3.10)
project(ACO_SFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wyszukaj pliki źródłowe
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Dodaj folder z nagłówkami
include_directories(${CMAKE_SOURCE_DIR}/include)

# Znajdź SFML (wymagane komponenty)
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)

# Zbuduj plik wykonywalny
add_executable(sfml_game ${SOURCES})

# Podlinkuj biblioteki SFML
target_link_libraries(sfml_game sfml-graphics sfml-window sfml-system)
