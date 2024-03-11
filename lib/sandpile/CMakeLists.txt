add_library(sandpile STATIC sandpile.cpp sandpile.h)

target_include_directories(sandpile PUBLIC ${PROJECT_SOURCE_DIR})