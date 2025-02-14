cmake_minimum_required(VERSION 3.14)

project(pcapReader LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(PkgConfig REQUIRED)
pkg_search_module(PCAP REQUIRED libpcap)

add_executable(pcapReader
  main.cpp
  pcapreader.h
  pcapreader.cpp
)

target_link_libraries(pcapReader PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_link_libraries(pcapReader PRIVATE ${PCAP_LIBRARIES})

target_include_directories(pcapReader PRIVATE ${PCAP_INCLUDE_DIRS})

install(TARGETS pcapReader
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
