#
# Created by Igor Filatov.
#

project(FeerieCore)
cmake_minimum_required(VERSION 3.0)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/feerie SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/feerie/collection SOURCES)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11") # -stdlib=libc++")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
)

if(UnitTests)
    add_subdirectory(tests)
endif()

BuildDocumentation()
