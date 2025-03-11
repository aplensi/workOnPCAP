cmake_minimum_required(VERSION 3.14)

project(pcapReader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pcapReader
  main.cpp
  ownpcapreader.cpp
  ownpcapreader.h
  readFile.cpp
  readFile.h
)
