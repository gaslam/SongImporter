cmake_minimum_required(VERSION 3.16)

project(SongImporterLib LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Concurrent)
add_library(SongImporterLib SHARED
  SongImporterLib_global.h
  SongImporterLib.cpp
  SongImporterLib.h
  OperationResult.h
  SongValidator.h
  IsFileValidator.h
  IsFileValidator.cpp
  IsFolderValidator.h
  IsFolderValidator.cpp
  SoftwareUtils.h
  SoftwareUtils.cpp
  FileUtils.h
  FileUtils.cpp
  Song.h
  BytesTracker.h
  BytesTracker.cpp
  SongAnalyzer.h
  SongAnalyzer.cpp
  Managers/AnalyzerManager.h Managers/AnalyzerManager.cpp
  Helpers/SongEventWrapper.h
)

target_compile_definitions(SongImporterLib PRIVATE SONGIMPORTERLIB_LIBRARY)

find_package(ZLIB REQUIRED)
target_include_directories(SongImporterLib PRIVATE ${ZLIB_INCLUDE_DIR})
target_link_directories(SongImporterLib PRIVATE ${ZLIB_LIBRARY})

find_package(QuaZip-Qt6 REQUIRED)
find_package(TagLib 2.0.0 REQUIRED)

target_link_libraries(SongImporterLib PUBLIC ZLIB::ZLIB QuaZip::QuaZip TagLib::tag)
target_link_libraries(SongImporterLib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Concurrent)

target_precompile_headers(SongImporterLib PUBLIC
    stdafx.h
)
