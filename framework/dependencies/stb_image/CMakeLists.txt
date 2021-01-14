
cmake_minimum_required(VERSION 3.12)
project(STB_IMAGE VERSION 0.0.0)


set(SOURCES
	stb_image.cpp
	stb_image.h
)

add_library(stb_image ${SOURCES})