# Locate PhysX
# This module defines XXX_FOUND, XXX_INCLUDE_DIRS and XXX_LIBRARIES standard variables
#
# Sets the ${TRGTNAME}_LIBRARIES variable according to debug and release libraries given.
function(FIND_PACKAGE_ADD_TARGET_LIBRARIES TRGTNAME RELEASE_LIB DEBUG_LIB)
        if(RELEASE_LIB)
                if(DEBUG_LIB)
                        list(APPEND ${TRGTNAME}_LIBRARIES optimized "${RELEASE_LIB}" debug "${DEBUG_LIB}")
                else()
                        list(APPEND ${TRGTNAME}_LIBRARIES "${RELEASE_LIB}")
                endif()
        endif()
        set(${TRGTNAME}_LIBRARIES ${${TRGTNAME}_LIBRARIES} PARENT_SCOPE)
endfunction()

set(bitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(bitness 64)
endif()

set(PHYSX_INCLUDE_DIRS)

if(APPLE)
   set(PHYSX_SEARCH_PATHS "/usr/local/physx" PhysX SDK)

elseif(WINDOWS)

    set(PHYSX_SDK_DEFAULT_PATH $ENV{PROGRAMFILES}/NVIDIA Corporation/NVIDIA PhysX SDK)
    set(
        PHYSX_SEARCH_PATHS
        ${PHYSX_SDK_DEFAULT_PATH}/v3.3/
    )

elseif(UNIX)
    set(PHYSX_SEARCH_PATHS "/usr/local/physx" physx PhysX SDK)
endif()

find_path(PHYSX_INCLUDE_DIR
   NAMES "PxPhysics.h"
   HINTS
   ${PHYSX_SEARCH_PATHS}
   PATH_SUFFIXES Include
)

set(PHYSX_INCLUDE_DIRS ${PHYSX_INCLUDE_DIR})

option(BUILD_PHYSX_AGAINST_CHECKED_LIBS "Build against the CHECKED libraries in physx" OFF)
option(BUILD_PHYSX_AGAINST_PROFILE_LIBS "Build against the PROFILE libraries in physx" OFF)

if(bitness EQUAL 64)
   set(PX_BUILD_64 ON)
   set(PX_BUILD_32 OFF)
else()
   set(PX_BUILD_64 OFF)
   set(PX_BUILD_32 ON)
endif()

if(APPLE)
    list(FIND CMAKE_OSX_ARCHITECTURES i386 PX_IDX)
    if (PX_IDX GREATER -1)
        set(PX_BUILD_32 ON)
    endif()

    list(FIND CMAKE_OSX_ARCHITECTURES x86_64 PX_IDX)

    if (PX_IDX GREATER -1)
        set(PX_BUILD_64 ON)
    endif()
endif()

SET(PHYSX_LIBS 
    LowLevel
    LowLevelCloth
    PhysX3
    PhysX3CharacterKinematic
    PhysX3Common
    PhysX3Cooking
    PhysX3Extensions
    PhysX3Vehicle
    PhysXProfileSDK
    PhysXVisualDebuggerSDK
    PvdRuntime
    PxTask
    SceneQuery
    SimulationController
    LowLevel_x64
    LowLevelCloth_x64
    PhysX3_x64
    PhysX3CharacterKinematic_x64
    PhysX3Common_x64
    PhysX3Cooking_x64
    PhysX3Extensions_x64
    PhysX3Vehicle_x64
    PhysXProfileSDK_x64
    PhysXVisualDebuggerSDK_x64
    PvdRuntime_x64
    PxTask_x64
    SceneQuery_x64
    SimulationController_x64)

macro(find_PX_LIB CUR_LIB)
    STRING(TOLOWER "${CUR_LIB}" CUR_LIB_LOWER)
   
    if(PX_BUILD_64)
        FIND_LIBRARY(PHYSX_LIBRARY_64_${CUR_LIB}
                     NAMES ${CUR_LIB} ${CUR_LIB_LOWER}
                     HINTS
                     ${PHYSX_SEARCH_PATHS}
                     PATH_SUFFIXES Lib/osx64 Lib/win64 Lib/linux64 Bin/osx64 Bin/win64 Bin/linux64
        )
   endif()

    if(PX_BUILD_32)
        FIND_LIBRARY(PHYSX_LIBRARY_32_${CUR_LIB}
                     NAMES ${CUR_LIB} ${CUR_LIB_LOWER}
                     HINTS
                     ${PHYSX_SEARCH_PATHS}
                     PATH_SUFFIXES Lib/osx32 Lib/win32 Lib/linux32 Bin/osx32 Bin/win32 Bin/linux32
        )
    endif()
endmacro()

foreach(CUR_LIB ${PHYSX_LIBS})
    if(BUILD_PHYSX_AGAINST_CHECKED_LIBS)
        find_PX_LIB(${CUR_LIB}CHECKED)
    elif(BUILD_PHYSX_AGAINST_PROFILE_LIBS)
        find_PX_LIB(${CUR_LIB}PROFILE)
    endif()
   
    if(NOT PHYSX_LIBRARY_64_${CUR_LIB})
        find_PX_LIB(${CUR_LIB})
    endif()

     # Combine all libs to one variable
    if(PX_BUILD_64 AND PHYSX_LIBRARY_64_${CUR_LIB})
        FIND_PACKAGE_ADD_TARGET_LIBRARIES(PHYSX "${PHYSX_LIBRARY_64_${CUR_LIB}}" "")
    endif()
    if(PX_BUILD_32 AND PHYSX_LIBRARY_32_${CUR_LIB})
        FIND_PACKAGE_ADD_TARGET_LIBRARIES(PHYSX "${PHYSX_LIBRARY_32_${CUR_LIB}}" "")
    endif()
endforeach()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PHYSX DEFAULT_MSG PHYSX_LIBRARIES PHYSX_INCLUDE_DIRS)

