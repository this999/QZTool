add_library(QZToolCore STATIC
    question.cpp
    question.h
    question_manager.h
    question_manager.cpp
    user.h
    user.cpp
    session.h
    session.cpp
)

target_link_libraries(QZToolCore PRIVATE Qt6::Core)
