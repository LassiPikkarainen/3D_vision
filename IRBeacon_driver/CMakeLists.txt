cmake_minimum_required(VERSION 3.16)

project(IRBeacon_driver VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appIRBeacon_driver
    main.cpp
)

qt_add_qml_module(appIRBeacon_driver
    URI IRBeacon_driver
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES serialwriter.h serialwriter.cpp
        SOURCES
        SOURCES ircontroller.h ircontroller.cpp
        SOURCES
        SOURCES
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appIRBeacon_driver PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appIRBeacon_driver
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appIRBeacon_driver
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appIRBeacon_driver
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
