cmake_minimum_required(VERSION 3.21)
project(2048_cpp LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(2048_cpp src/main.cpp
src/gameBoard.cpp
src/gameBoard.h
src/tile.cpp
src/tile.h
src/RoundedRectangleShape.cpp
src/RoundedRectangleShape.hpp
)
target_link_libraries(2048_cpp PRIVATE sfml-graphics)
target_compile_features(2048_cpp PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET 2048_cpp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:2048_cpp> $<TARGET_FILE_DIR:2048_cpp> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS 2048_cpp)
