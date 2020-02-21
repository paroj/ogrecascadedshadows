cmake_minimum_required (VERSION 2.8.3)
project(StableCSMdemo)

set(APP_NAME StableCSMDemo)

file(GLOB SOURCE_FILES src/*.cpp src/*.h)

find_package(OGRE REQUIRED)
add_executable(${APP_NAME} ${SOURCE_FILES})
target_link_libraries (${APP_NAME} OgreBites)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/resources.cfg" "\
    [General]
    FileSystem=${PROJECT_SOURCE_DIR}/media/core
    FileSystem=${PROJECT_SOURCE_DIR}/media/example")
