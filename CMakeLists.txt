cmake_minimum_required (VERSION 2.8.3)
project(StableCSMdemo)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

set(CMAKE_PREFIX_PATH $ENV{OGRE_DEPENDENCIES_DIR} ${CMAKE_PREFIX_PATH}) # helps find Ogre's dependencies

add_subdirectory (src)
