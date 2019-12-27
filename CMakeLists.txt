project(GameboyEmulator)

cmake_minimum_required(VERSION 3.4.1)
add_definitions(
    -std=gnu++11
    -fexceptions
    -frtti
    -DUNIX
)

add_executable(
    gameboyEmulator
    src/CPU.cpp
    src/main.cpp
    src/Log.cpp
)

target_include_directories(
    gameboyEmulator
    PUBLIC inc
)

target_link_libraries(
    gameboyEmulator
)

add_executable(
    tests
    src/CPU.cpp
    src/Log.cpp
    test/main.cpp
    test/SimpleMemory.cpp
)

target_include_directories(
    tests
    PUBLIC inc
    PUBLIC test/test_inc
)

target_link_libraries(
    tests
)
