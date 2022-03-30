cmake_minimum_required(VERSION 3.12)
project(GraphicLearnAndTest)

#platform
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(LINUX ON)
    add_definitions(-DLINUX)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(WIN ON)
    add_definitions(-DWIN)
elseif(APPLE)
    set(MACOS ON)
    add_definitions(-DMACOS)
endif()

set(SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/SDK)
set(SDK_COMMON_PATH ${SDK_PATH}/Common)

if(LINUX)
    set(SDK_PLATFORM_PATH ${SDK_PATH}/Platform/linux)
elseif(WIN)
    set(SDK_PLATFORM_PATH ${SDK_PATH}/Platform/win)
elseif(MACOS)
    set(SDK_PLATFORM_PATH ${SDK_PATH}/Platform/macos)
endif()


set(OSGTest ON)

#judge current compile type
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-D_DEBUG)
    set(G_DEBUG ON)
    message("Debug version")
elseif((CMAKE_BUILD_TYPE MATCHES "Release"))
    add_definitions(-D_RELEASE)
    set(G_RElEASE ON)
    message("Release version")
elseif(CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
    add_definitions(-D_RELWITHDEBUG)
    set(G_RELWITHDEBUG ON)
    message("relWithDebug version")
endif()

# project in Linux
if(LINUX)
elseif(WIN)
elseif(MACOS)
endif()

#common project
if(LINUX OR WIN OR MACOS)
    add_subdirectory(OSGTest)
endif()

