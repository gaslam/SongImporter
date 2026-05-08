cmake_minimum_required(VERSION 3.16)
project(SongImporter)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ThirdParty/quazip/lib/cmake/QuaZip-Qt6-1.5")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ThirdParty/zlib/lib")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ThirdParty/zlib/include")

add_subdirectory(SongImporterGui)
add_subdirectory(SongImporterLib)
