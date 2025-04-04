add_library(QZToolCore STATIC
    question.cpp
    question.h
)

target_link_libraries(QZToolCore PRIVATE Qt6::Core)
