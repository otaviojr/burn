# Generated from basic.pro.

cmake_minimum_required(VERSION 3.14)
project(burn LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QT_QML_DEBUG 1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_DIR)
  set(INSTALL_DIR "/app")
endif()

set(INSTALL_DIR "${INSTALL_DIR}/bin")

add_subdirectory(src/gerber)

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Qml)
find_package(Qt6 COMPONENTS Quick)
find_package(Qt6 COMPONENTS QuickControls2)
find_package(Qt6 COMPONENTS WebSockets)

qt_add_executable(burn
    src/main.cpp
    src/gerber_renderer.cpp
    src/gerber_renderer_mirror.cpp
)

set_target_properties(burn PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

link_directories("${PROJECT_BINARY_DIR}/")

target_link_libraries(burn PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::QuickControls2
    Qt::WebSockets
    gerbv
)

# special case begin
if(UNIX AND NOT APPLE AND CMAKE_CROSSCOMPILING)
    find_package(Qt6 COMPONENTS QuickTemplates2)
     # Work around QTBUG-86533
    target_link_libraries(gallery_controls2 PRIVATE Qt::QuickTemplates2)
endif()
# special case end

# Resources:
set(burn_resource_files
    "resources/main.qml"
    "resources/main_cli.qml"
    "resources/BurnButton.qml"
    "resources/pages/FolderSelection.qml"
    "resources/pages/FileSelection.qml"
    "resources/pages/ConfigDialog.qml"
    "resources/images/v1/folder.png"
    "resources/images/v1/file.png"
    "resources/qtquickcontrols2.conf"
)

qt6_add_resources(burn "burn"
    PREFIX
        "/"
    FILES
        ${burn_resource_files}
)

if(QT_FEATURE_static) # special case
    find_package(Qt6 COMPONENTS Svg) # special case
    target_link_libraries(burn PUBLIC
        Qt::Svg
    )
endif()

install(TARGETS burn
    RUNTIME DESTINATION "${INSTALL_DIR}"
    BUNDLE DESTINATION "${INSTALL_DIR}"
    LIBRARY DESTINATION "${INSTALL_DIR}"
)
