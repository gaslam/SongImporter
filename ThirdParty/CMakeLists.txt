include_directories()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ThirdParty/zlib/lib")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ThirdParty/zlib/include")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ThirdParty/quazip/lib/cmake/QuaZip-Qt6-1.5")

# variables for github actions

set(ZLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/thirdparty/zlib/include)
set(ZLIB_LIBRARY ${CMAKE_SOURCE_DIR}/ThirdParty/zlib/lib/libz.dll.a)
set(QuaZip-Qt6_DIR ${CMAKE_SOURCE_DIR}/ThirdParty/quazip/lib/cmake/QuaZip-Qt6-1.5)
