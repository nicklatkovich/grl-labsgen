cmake_minimum_required(VERSION 3.1)
project(GrL-LabsGen)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(
	GrL-LabsGen
	main.cpp
	src/Chamber.cpp
	src/ChamberGener.cpp
	src/Grid.cpp
	src/Point.cpp
	src/UnorderedArray.cpp
)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/dist/maps)
