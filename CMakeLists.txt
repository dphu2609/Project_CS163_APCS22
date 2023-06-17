cmake_minimum_required(VERSION 3.0.0)
project(Project_CS163_APCS22 VERSION 0.1.0 LANGUAGES C CXX)

# Locate SFML

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

include(CTest)
enable_testing()

# Collect all source files in the source directory
file(GLOB SOURCES "source/*.cpp")

add_executable(Project_CS163_APCS22 ${SOURCES})

# Add include directories
target_include_directories(Project_CS163_APCS22 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/header")

# Link SFML libraries to the executable
target_link_libraries(Project_CS163_APCS22 sfml-graphics sfml-audio)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
