#
# Source
#
file(GLOB_RECURSE SRC 
"./*.cpp"

)
add_library(${PROJECT_NAME}_trgt STATIC ${SRC})

include_directories(${CMAKE_SOURCE_DIR}/external/)



 

install(
    FILES gum-maths.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/
) 

file(GLOB HEADERS Maths/*.h)
install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Maths/
)




set(COMBINED_LIB ${CMAKE_BINARY_DIR}/lib${PROJECT_NAME}.a)

add_custom_target(${PROJECT_NAME}_createcombineddir
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/combined/
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/
    DEPENDS ${PROJECT_NAME}_trgt
    VERBATIM
)

add_custom_command( #combinedtarget ALL
    OUTPUT ${COMBINED_LIB}
    
    COMMAND ar -x $<TARGET_FILE:${PROJECT_NAME}_trgt>
    COMMAND sh -c "ar -qcs \"${COMBINED_LIB}\" *.o"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/combined/
    DEPENDS  ${PROJECT_NAME}_createcombineddir ${PROJECT_NAME}_trgt
    COMMENT "Combining libs"
    VERBATIM
)

add_custom_target(${PROJECT_NAME}_combinedlibtarget ALL DEPENDS ${COMBINED_LIB})

# Create an library target out of the library compilation result
add_library(${PROJECT_NAME} STATIC IMPORTED GLOBAL)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_combinedlibtarget)

# Specify where the library is and where to find the headers
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    IMPORTED_LOCATION "${COMBINED_LIB}"
)

install(FILES ${COMBINED_LIB}
    DESTINATION ${CMAKE_INSTALL_LIBDIR})

#install(
#    EXPORT ${PROJECT_NAME}Targets
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
#    FILE ${PROJECT_NAME}Targets.cmake
#)

configure_package_config_file("Config.cmake.in"
    "${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    PATH_VARS CMAKE_INSTALL_LIBDIR
)

#write_basic_package_version_file(
#	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
#	VERSION 1.0.0
#	COMPATIBILITY SameMajorVersion
#)

### Install Config and ConfigVersion files
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#          "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

export(PACKAGE ${PROJECT_NAME})

