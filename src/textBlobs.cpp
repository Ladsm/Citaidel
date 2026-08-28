#include "textBlobs.hpp"
std::string make_binary(const std::string& project_name) {
    return "cmake_minimum_required(VERSION 3.20)\n"
        "project(" + project_name + " VERSION 1.0 LANGUAGES CXX)\n"
        R"(
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BIN_SOURCES
    src/main.cpp
    #CITAIDEL_ANCHOR
)
add_executable(${PROJECT_NAME} ${BIN_SOURCES})
target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.h)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    target_link_libraries(${PROJECT_NAME} PRIVATE user32 gdi32 shell32 advapi32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()
)";
}

std::string make_library(const std::string& library_name) {
    return "cmake_minimum_required(VERSION 3.20)\n"
        "project(" + library_name + " VERSION 1.0 LANGUAGES CXX)\n"
        R"(
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(LIB_SOURCES
    src/lib.cpp
    #CITAIDEL_ANCHOR
)
add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.h)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
    target_link_libraries(${PROJECT_NAME} PUBLIC user32 gdi32 shell32 advapi32)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
endif()
add_executable(${PROJECT_NAME}_demo demo/demo.cpp)
target_link_libraries(${PROJECT_NAME}_demo PRIVATE ${PROJECT_NAME})
target_precompile_headers(${PROJECT_NAME}_demo PRIVATE include/pch.h)
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(FILES cmake/${PROJECT_NAME}Config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
)";
}

std::string make_config(const std::string& project_name) {
    return "include(\"${CMAKE_CURRENT_LIST_DIR}/" + project_name + "Targets.cmake\")\n"
        R"(
if(NOT WIN32)
    find_package(Threads REQUIRED)
endif()
)";
}