add_library(bmp STATIC bmp.cpp bmp.h)

target_include_directories(bmp PUBLIC ${PROJECT_SOURCE_DIR})