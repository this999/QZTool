include_directories(${CMAKE_SOURCE_DIR})

add_executable(QZToolTests
    question_test.cpp
    question_manager_test.cpp
)

target_link_libraries(QZToolTests PRIVATE Qt6::Core Qt6::Widgets QZToolCore gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(QZToolTests)
