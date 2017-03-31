cmake_minimum_required(VERSION 2.8.11)
project( bbb_robot )
message( ${CMAKE_C_COMPILER} )
message( ${CMAKE_CXX_COMPILER} )

# set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} cmake )
set( VERSION_MAJOR   1  CACHE STRING "Project major version number." )
set( VERSION_MINOR   0  CACHE STRING "Project minor version number." )
set( VERSION_PATCH   0  CACHE STRING "Project patch version number." )
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# find_library( LAPACK lapack)
# find_library( PTHREAD pthread )

## set output directory ##
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/ )

# add_definitions(-ggdb)
# add_definitions(-std=c11)
# add_definitions(-O3)
# add_definitions(-Wall)
# add_definitions(-D_GNU_SOURCE=1)
# add_definitions(-pipe)
# add_definitions(-fPIC)
add_definitions(-D__AVR__)

include_directories(
    ${bbb_robot_SOURCE_DIR}/application
)

## build static library ###
add_library(nxshield STATIC
    ${bbb_robot_SOURCE_DIR}/src/AbsoluteIMU.cpp
    ${bbb_robot_SOURCE_DIR}/src/ACCLNx.cpp
    ${bbb_robot_SOURCE_DIR}/src/AngleSensor.cpp
    ${bbb_robot_SOURCE_DIR}/src/BaseI2CDevice.cpp
    ${bbb_robot_SOURCE_DIR}/src/DISTNx.cpp
    ${bbb_robot_SOURCE_DIR}/src/EV3Color.cpp
    ${bbb_robot_SOURCE_DIR}/src/EV3Gyro.cpp
    ${bbb_robot_SOURCE_DIR}/src/EV3InfraRed.h
    ${bbb_robot_SOURCE_DIR}/src/EV3SensorAdapter.cpp
    ${bbb_robot_SOURCE_DIR}/src/EV3Sonar.cpp
    ${bbb_robot_SOURCE_DIR}/src/LineLeader.cpp
    ${bbb_robot_SOURCE_DIR}/src/MagicWand.cpp
    ${bbb_robot_SOURCE_DIR}/src/MsTimer2.cpp
    ${bbb_robot_SOURCE_DIR}/src/NumericPad.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXShield.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXShieldAGS.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXShieldI2C.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXTCam.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXTCurrentMeter.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXTHID.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXTLight.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXTMMX.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXTPowerMeter.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXTServo.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXTTouch.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXTUS.cpp
    ${bbb_robot_SOURCE_DIR}/src/NXTVoltMeter.cpp
    ${bbb_robot_SOURCE_DIR}/src/PFMate.cpp
    ${bbb_robot_SOURCE_DIR}/src/PiLight.cpp
    ${bbb_robot_SOURCE_DIR}/src/PSPNx.cpp
    ${bbb_robot_SOURCE_DIR}/src/RCXLight.cpp
    ${bbb_robot_SOURCE_DIR}/src/RTC.cpp
    ${bbb_robot_SOURCE_DIR}/src/SoftI2cMaster.cpp
    ${bbb_robot_SOURCE_DIR}/src/SumoEyes.cpp
)


### build applications ###
add_executable(
  nxshield_test
  ${bbb_robot_SOURCE_DIR}/application/motor_and_touch.cpp
  # ${bbb_robot_SOURCE_DIR}/application/i2c_utils.cpp
  # ${bbb_robot_SOURCE_DIR}/application/nxtalk.cpp
)

target_link_libraries(nxshield_test nxshield)
