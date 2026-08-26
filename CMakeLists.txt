cmake_minimum_required(VERSION 3.20)
project(Citaidel VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BIN_SOURCES
    src/main.cpp
    #CGEN_ANCHOR
    src/textBlobs.cpp
    src/projectManager.cpp
)
add_executable(${PROJECT_NAME} ${BIN_SOURCES})
target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.h)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
find_package(LITW REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE LITW::LITW)
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    target_link_libraries(${PROJECT_NAME} PRIVATE user32 gdi32 shell32 advapi32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()
