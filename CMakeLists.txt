project (nes)
cmake_minimum_required (VERSION 2.6)

set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/nes)

set(NES_INC "${SRC_ROOT}/inc/")
set(NES_SRC
   	"${SRC_ROOT}/src/cpu.cpp"
	"${SRC_ROOT}/src/ines.cpp"
	"${SRC_ROOT}/src/main.cpp"
)

set(NES_COMPILE_FLAGS
	-std=c++11
	-pedantic
	-Wall
	-Wextra
)

include_directories(${NES_INC})
add_definitions(${NES_COMPILE_FLAGS})
add_executable(nes ${NES_SRC})
