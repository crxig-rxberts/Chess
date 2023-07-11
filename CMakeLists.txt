cmake_minimum_required(VERSION 3.25)
project(chess2)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_package(Catch2 REQUIRED)

include_directories(${SFML_INCLUDE_DIR} include)

add_executable(chess2
        src/main/main.cpp
        src/main/game/Board.cpp
        src/main/game/Piece.cpp
        src/main/game/PieceMovement.cpp
        src/main/game/BoardEvent.cpp
        include/BoardEvent.hpp
        src/main/menu/Menu.cpp include/Menu.hpp)

target_link_libraries(chess2
        sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio
        )
