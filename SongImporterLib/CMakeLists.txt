cmake_minimum_required(VERSION 3.16)

project(SongImporterLib LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(SongImporterLib STATIC
  SongImporterLib.cpp
  SongImporterLib.h
 OperationResult.h
 Rule.h
 IsFileRule.h
 IsFileRule.cpp)

target_link_libraries(SongImporterLib PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(SongImporterLib PRIVATE SONGIMPORTERLIB_LIBRARY)

target_precompile_headers(SongImporterLib PUBLIC stdafx.h)
