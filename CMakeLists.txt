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
    src/Cartridge.cpp
    src/CPU.cpp
    src/GameboyMotherboard.cpp
    src/main.cpp
    src/Log.cpp
    src/Motherboard.cpp
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
    src/Cartridge.cpp
    src/CPU.cpp
    src/GameboyMotherboard.cpp
    src/Log.cpp
    src/Motherboard.cpp
    test/main.cpp
    test/SimpleMotherboard.cpp
)

target_include_directories(
    tests
    PUBLIC inc
    PUBLIC test/test_inc
)

target_link_libraries(
    tests
)

enable_testing()

add_test(unit_tests tests)
