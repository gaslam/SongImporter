cmake_minimum_required(VERSION 3.16)

project(SongImporterGui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Svg)


set(CMAKE_AUTORCC ON)

qt_standard_project_setup(REQUIRES 6.8)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/Resources/resources.rc")
    qt_add_executable(appSongImporterGui main.cpp ${app_icon_resource_windows} ${CMAKE_CURRENT_SOURCE_DIR}/Resources/resources.qrc)

set_source_files_properties(Theme.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(appSongImporterGui
    URI SongImporterGui
    RESOURCES Resources/resources.qrc
    QML_FILES
        Main.qml
        Theme.qml
        ImporterButton.qml
        InputField.qml
        InputFieldButton.qml
        FileDropArea.qml
        SongExportForm.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appSongImporterGui PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appSongImporterGui
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appSongImporterGui
    PUBLIC ${CMAKE_SOURCE_DIR}/SongImporterLib
)

target_link_libraries(appSongImporterGui
    PUBLIC SongImporterLib Qt6::Quick Qt6::Svg
)

target_precompile_headers(appSongImporterGui PUBLIC stdafx.h)

include(GNUInstallDirs)
install(TARGETS appSongImporterGui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
