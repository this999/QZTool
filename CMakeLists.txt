cmake_minimum_required(VERSION 3.5)

project(QZTool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

include_directories(${CMAKE_SOURCE_DIR}/QZToolCore)

add_subdirectory(QZToolCore)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QZTool
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        QZToolCore/question.cpp
        QZToolCore/question.h
        QZToolCore/question_manager.h
        QZToolCore/question_manager.cpp
    )
else()
    add_executable(QZTool ${PROJECT_SOURCES} QZToolCore/question.cpp QZToolCore/question.h)
endif()

target_link_libraries(QZTool PRIVATE Qt${QT_VERSION_MAJOR}::Widgets QZToolCore)

enable_testing()
add_subdirectory(tests)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QZTool)
endif()

include(GNUInstallDirs)
install(TARGETS QZTool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
