cmake_minimum_required(VERSION 2.8.11)
project( bbb_robot )
message( ${CMAKE_C_COMPILER} )
message( ${CMAKE_CXX_COMPILER} )

# set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} cmake )
set( VERSION_MAJOR   1  CACHE STRING "Project major version number." )
set( VERSION_MINOR   0  CACHE STRING "Project minor version number." )
set( VERSION_PATCH   0  CACHE STRING "Project patch version number." )
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# find_library( PTHREAD pthread )

## set output directory ##
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/ )

add_definitions(-ggdb)
# add_definitions(-std=c11)
# add_definitions(-O3)
add_definitions(-Wall)
# add_definitions(-D_GNU_SOURCE=1)
# add_definitions(-pipe)
add_definitions(-fPIC)
# add_definitions(-D__AVR__)

include_directories(
    ${bbb_robot_SOURCE_DIR}/src
)

## build static library ###
add_library(nxshield STATIC
    ${bbb_robot_SOURCE_DIR}/src/BaseI2CDevice.cpp
    ${bbb_robot_SOURCE_DIR}/src/NxShield.cpp
)

include_directories(
    ${bbb_robot_SOURCE_DIR}/src
)

### build applications ###
add_executable(
  nxshield_test
  ${bbb_robot_SOURCE_DIR}/application/main.cpp
)

target_link_libraries(nxshield_test nxshield)
