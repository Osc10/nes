project (nes)
cmake_minimum_required (VERSION 2.6)

set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(NES_INC "${SRC_ROOT}/inc/")
set(NES_SRC
        "${SRC_ROOT}/src/cpu.cpp"
        "${SRC_ROOT}/src/controller.cpp"
        "${SRC_ROOT}/src/emulator.cpp"
        "${SRC_ROOT}/src/ines.cpp"
        "${SRC_ROOT}/src/main.cpp"
        "${SRC_ROOT}/src/ppu.cpp"
        "${SRC_ROOT}/src/screen.cpp"
)

set(NES_COMPILE_FLAGS
        -std=c++17
        -g
        -pedantic
        -Wall
        -Wextra
        -DNDEBUG
)

find_package(SDL2 REQUIRED)
include_directories(${NES_INC} ${SDL2_INCLUDE_DIRS})
add_definitions(${NES_COMPILE_FLAGS})
add_executable(nes ${NES_SRC})
target_link_libraries(nes ${SDL2_LIBRARIES})
