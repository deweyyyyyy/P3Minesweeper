cmake_minimum_required(VERSION 3.28)
project(P3Minesweeper LANGUAGES CXX)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)


add_executable(P3Minesweeper
        src/main.cpp
        src/WelcomeWindow.cpp
        src/WelcomeWindow.h
        src/GameWindow.cpp
        src/GameWindow.h
        src/Board.cpp
        src/Board.h
        src/TextureManager.cpp
        src/TextureManager.h
        src/LeaderboardWindow.cpp
        src/LeaderboardWindow.h
)


target_compile_features(P3Minesweeper PRIVATE cxx_std_17)
target_link_libraries(P3Minesweeper PRIVATE SFML::Graphics SFML::Window SFML::System)
