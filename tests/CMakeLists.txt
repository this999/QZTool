include_directories(${CMAKE_SOURCE_DIR})

add_executable(QZToolTests
    question_test.cpp
    question_manager_test.cpp
    user_test.cpp
    session_test.cpp
)

target_link_libraries(QZToolTests PRIVATE Qt6::Core Qt6::Widgets QZToolCore gtest gtest_main)

# Define UNIT_TESTING for test targets so test-only APIs can be guarded
target_compile_definitions(QZToolTests PRIVATE UNIT_TESTING)
target_compile_definitions(QZToolCore PRIVATE UNIT_TESTING)

include(GoogleTest)
gtest_discover_tests(QZToolTests)
