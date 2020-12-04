cmake_minimum_required(VERSION 3.12)

project(nativefiledialog VERSION 0.0.0)

set(SOURCES
	src/nfd_win.cpp
	src/nfd_common.c
	src/include/nfd.h
)

add_library(nativefiledialog STATIC ${SOURCES})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src/include
)