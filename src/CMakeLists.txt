set(APP_NAME StableCSMDemo)

file(GLOB SOURCE_FILES *.cpp *.h)

find_package(Boost COMPONENTS thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(OGRE_SOURCE $ENV{OGRE_SOURCE})
find_package(OGRE REQUIRED)
include_directories(${OGRE_INCLUDE_DIRS})
include_directories(${OGRE_PREFIX_DEPENDENCIES_DIR}/include)

find_package(OIS REQUIRED)
include_directories(${OIS_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIRS})


add_executable(${APP_NAME} ${SOURCE_FILES})

target_link_libraries (${APP_NAME} ${OGRE_LIBRARIES} ${OIS_LIBRARIES})
