cmake_minimum_required(VERSION 3.12)
project(OSGTest)

#preDefine
add_definitions(-DCURRENT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})

#load SDK
#osg
if(G_DEBUG)
    set(OSG_LIB_PATH ${SDK_PLATFORM_PATH}/osg/debug)
elseif(G_RElEASE)
    set(OSG_LIB_PATH ${SDK_PLATFORM_PATH}/osg/release)
    message("release version")
    message(STATUS ${OSG_LIB_PATH})
elseif(G_RELWITHDEBUG)
    set(OSG_LIB_PATH ${SDK_PLATFORM_PATH}/osg/relWithDebug)
endif()
message(STATUS ${OSG_LIB_PATH})
include_directories(${OSG_LIB_PATH}/include)
set(OSG_LIB ${OSG_LIB_PATH}/lib)
if(WIN)
    FILE(GLOB OSG_RUN_TIME_DLLs ${OSG_LIB_PATH}/bin/*.dll ${OSG_LIB_PATH}/bin/*.pdb)
    FILE(COPY ${OSG_RUN_TIME_DLLs} DESTINATION ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/)
endif()

# files need to be compiled
aux_source_directory(./ ROOT_DIR_FILES)



add_executable(${PROJECT_NAME} ${ROOT_DIR_FILES})
target_link_directories(${PROJECT_NAME} PUBLIC ${OSG_LIB})
if(WIN)
    target_link_libraries(${PROJECT_NAME} PUBLIC opengl32)
endif()

if(G_DEBUG)
    target_link_libraries(${PROJECT_NAME} PUBLIC osgDBd osgViewerd OpenThreadsd osgd osgGAd osgUtild)
elseif(G_RElEASE)
    target_link_libraries(${PROJECT_NAME} PUBLIC osgDB osgViewer OpenThreads osg osgGA osgUtil)
elseif(G_RELWITHDEBUG)
    target_link_libraries(${PROJECT_NAME} PUBLIC osgDBrd osgViewerrd OpenThreadsrd osgrd osgGArd osgUtilrd)
endif()