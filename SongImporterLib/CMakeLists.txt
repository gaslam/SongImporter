cmake_minimum_required(VERSION 3.16)

project(SongImporterLib LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Concurrent Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Concurrent Quick)

set (utils_folder "Utils")
set (validators_folder "Validators")
set (song_folder "Song")
set (managers_folder "Managers")
set (providers_folder "Providers")

set(cpp_files
        ${utils_folder}/BytesTracker.h
        ${utils_folder}/BytesTracker.cpp
        ${utils_folder}/FileUtils.h
        ${utils_folder}/FileUtils.cpp
        ${utils_folder}/SoftwareUtils.h
        ${utils_folder}/SoftwareUtils.cpp
        ${utils_folder}/OperationResult.h
        ${validators_folder}/SongValidator.h
        ${validators_folder}/IsFileValidator.h
        ${validators_folder}/IsFileValidator.cpp
        ${validators_folder}/IsFolderValidator.h
        ${validators_folder}/IsFolderValidator.cpp
        ${song_folder}/Song.h
        ${song_folder}/SongAnalyzer.h
        ${song_folder}/SongAnalyzer.cpp
        ${managers_folder}/AnalyzerManager.h
        ${managers_folder}/AnalyzerManager.cpp
        ${providers_folder}/AlbumCoverProvider.h
        ${providers_folder}/AlbumCoverProvider.cpp
    )


add_library(SongImporterLib SHARED

  ${cpp_files}
  SongImporterLib_global.h
)

target_compile_definitions(SongImporterLib PRIVATE SONGIMPORTERLIB_LIBRARY)

find_package(ZLIB REQUIRED)
target_include_directories(SongImporterLib PRIVATE ${ZLIB_INCLUDE_DIR})
target_link_directories(SongImporterLib PRIVATE ${ZLIB_LIBRARY})

find_package(QuaZip-Qt6 REQUIRED)
find_package(TagLib 2.0.0 REQUIRED)

target_link_libraries(SongImporterLib PUBLIC ZLIB::ZLIB QuaZip::QuaZip TagLib::tag)
target_link_libraries(SongImporterLib PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Concurrent)

set(SONGIMPORTERLIB_EXTRACT_ALBUMCOVERS ON CACHE BOOL "Enable album cover art extraction.")

if(SONGIMPORTERLIB_EXTRACT_ALBUMCOVERS)
  target_compile_definitions(SongImporterLib PUBLIC SONGIMPORTERLIB_EXTRACT_ALBUMCOVERS=1)
endif()

target_precompile_headers(SongImporterLib PUBLIC
    stdafx.h
)

target_include_directories(SongImporterLib PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
