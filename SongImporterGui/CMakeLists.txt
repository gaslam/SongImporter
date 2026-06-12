cmake_minimum_required(VERSION 3.16)

project(SongImporterGui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Svg)

set(CMAKE_AUTORCC ON)

qt_standard_project_setup(REQUIRES 6.8)

set(app_icon "")

if(WIN32)
    set(app_icon "${CMAKE_CURRENT_SOURCE_DIR}/Resources/resources.rc")
endif()

qt_add_executable(appSongImporterGui
   main.cpp ${app_icon} Resources/resources.qrc
)

set (components_folder "Components")
set (forms_folder "Forms")
set (styles_folder "Styles")
set (models_folder "Models")

set(plain_qml_files
    Main.qml
    ${components_folder}/ImporterButton.qml
    ${components_folder}/InputField.qml
    ${components_folder}/InputFieldButton.qml
    ${components_folder}/FileDropArea.qml
    ${forms_folder}/SongExportForm.qml
)
set(qml_singletons
       ${styles_folder}/Theme.qml
)

set(cpp_files
        ${models_folder}/SongModel.h
        ${models_folder}/SongModel.cpp
    )

set_source_files_properties(${qml_singletons}
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(appSongImporterGui
    URI SongImporterGui
    QML_FILES ${plain_qml_files} ${qml_singletons}
    SOURCES ${cpp_files}
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
    PRIVATE SongImporterLib Qt6::Quick Qt6::Svg
)

target_precompile_headers(appSongImporterGui PUBLIC
    stdafx.h
)

set(SONGIMPORTERLIB_EXTRACT_ALBUMCOVERS ON CACHE BOOL "Enable album cover art extraction." FORCE)

include(GNUInstallDirs)
install(TARGETS appSongImporterGui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
