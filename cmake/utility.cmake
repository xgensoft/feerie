macro(AddUnitTest PATH LIBRARIES TARGETS)
    get_filename_component(PROJECT ${PATH} NAME_WE)
    add_executable(${PROJECT} ${CMAKE_CURRENT_SOURCE_DIR}/${PATH})
    if (DEFINED GTEST_LOCAL_MAIN_LIBRARIES)
        target_link_libraries(${PROJECT} ${GTEST_LOCAL_MAIN_LIBRARIES} ${GTEST_LOCAL_LIBRARIES})
    else()
        target_link_libraries(${PROJECT} ${GTEST_MAIN_LIBRARIES} ${GTEST_LIBRARIES})
    endif()
    if(NOT LIBRARIES EQUAL "")
        target_link_libraries(${PROJECT} ${LIBRARIES})
    endif()

    set(${TARGETS} ${${TARGETS}} ${PROJECT})

    set_target_properties(${PROJECT}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    )

    add_test(${PROJECT} "${CMAKE_CURRENT_BINARY_DIR}/bin/${PROJECT}")

endmacro(AddUnitTest)

macro(UnitTests PATHES LIBRARIES)
    enable_testing()
    set(TARGETS "" PARENT_SCOPE)

# google test
    if (DEFINED GTEST_LOCAL_MAIN_LIBRARIES)
        message(STATUS "Using user-defined gtest ${GTEST_LOCAL_MAIN_LIBRARIES}")
        include_directories(${GTEST_LOCAL_INCLUDE_DIRS})
    else()
        message(STATUS "Using system gtest")
        find_package(GTest REQUIRED)
        include_directories(${GTEST_INCLUDE_DIRS})
    endif()

# add tests
    foreach(PATH ${PATHES})
        AddUnitTest("${PATH}" "${LIBRARIES}" TARGETS)
    endforeach()

# exec
    add_custom_target(all_tests ALL DEPENDS ${TARGETS})
    add_custom_command(TARGET all_tests
                       POST_BUILD COMMAND ctest ARGS --output-on-failure
    )

endmacro()

macro(BuildDocumentation)
    find_package(Doxygen)

    if(NOT DOXYGEN_FOUND)
        message("Doxygen is needed to build the documentation.")
    else()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )

#        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
    endif()
endmacro()

