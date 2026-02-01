cmake_minimum_required(VERSION 3.5)

project(QZTool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional coverage build flags and helper targets
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage flags")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    endif()
endif()

# Prefer system-provided GoogleTest, fallback to FetchContent if missing
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    message(STATUS "GTest not found: fetching via FetchContent")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(googletest)
else()
    message(STATUS "Using system-provided GTest")
endif()

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

# Helper targets for coverage generation/cleanup
add_custom_target(coverage-report
    COMMAND ${CMAKE_COMMAND} -E echo "Generating coverage report..."
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR} gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o coverage.html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generate HTML coverage report (requires gcovr)"
)

add_custom_target(clean-coverage
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaning coverage artifacts..."
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} ${CMAKE_COMMAND} -E env bash ${CMAKE_SOURCE_DIR}/scripts/clean-coverage.sh --full
    COMMENT "Clean coverage artifacts in build directory"
)

# Run coverage check script (prints summary, optional --threshold and --html)
add_custom_target(check-coverage
    COMMAND ${CMAKE_COMMAND} -E echo "Running coverage check..."
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} ${CMAKE_COMMAND} -E env bash ${CMAKE_SOURCE_DIR}/scripts/check-coverage.sh
    COMMENT "Run scripts/check-coverage.sh to print coverage summary"
)
