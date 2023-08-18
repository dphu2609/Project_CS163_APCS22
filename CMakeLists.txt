cmake_minimum_required(VERSION 3.0.0)
project(Project_CS163_APCS22 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

# Locate SFML
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

include(CTest)
enable_testing()

# Collect source files from multiple directories
file(GLOB STATESTACK_SOURCES "source/StateStack/*.cpp")
file(GLOB AVL_SOURCES "source/AVL/*.cpp")
file(GLOB TREE234_SOURCES "source/234Tree/*.cpp")
file(GLOB HEAP_SOURCES "source/Heap/*.cpp")
file(GLOB TRIE_SOURCES "source/Trie/*.cpp")
file(GLOB GRAPH_SOURCES "source/Graph/*.cpp")
file(GLOB HASHTABLE_SOURCES "source/HashTable/*.cpp")
file(GLOB SCENEGRAPH_SOURCES "source/SceneGraph/*.cpp")
file(GLOB MAIN_SOURCES "source/*.cpp")
file(GLOB MAIN_C_SOURCES "source/*.c")

# Create a single list of all source files
set(SOURCES ${STATESTACK_SOURCES} ${AVL_SOURCES} ${TREE234_SOURCES} ${HEAP_SOURCES} ${TRIE_SOURCES} ${GRAPH_SOURCES} ${HASHTABLE_SOURCES} ${SCENEGRAPH_SOURCES} ${MAIN_SOURCES} ${MAIN_C_SOURCES})

add_executable(Project_CS163_APCS22 WIN32 ${SOURCES})

# Add include directories
target_include_directories(Project_CS163_APCS22 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/header" "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/include")

target_compile_definitions(Project_CS163_APCS22 PRIVATE
    SFML_STATIC
)

# Link SFML libraries to the executable (static versions)
target_link_libraries(Project_CS163_APCS22 PRIVATE
    -static-libgcc
    -static-libstdc++
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    opengl32
    freetype
    winmm
    gdi32
)

# Enable specific flags
target_compile_options(Project_CS163_APCS22 PUBLIC -fdiagnostics-color=always -g -mwindows)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
