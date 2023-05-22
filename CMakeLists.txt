cmake_minimum_required(VERSION 3.25)
project(chess2)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

include_directories(${SFML_INCLUDE_DIR} include)

add_executable(chess2 src/main.cpp src/Board.cpp src/Chess.cpp src/Piece.cpp src/PieceMovement.cpp)

target_link_libraries(chess2 sfml-system sfml-window sfml-graphics sfml-network sfml-audio)