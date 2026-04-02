#
# Source
#
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
add_library(${CMAKE_PROJECT_NAME} STATIC ${SRC})

include_directories(${CMAKE_SOURCE_DIR}/external/)

install(
  FILES gum-maths.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/
) 

file(GLOB HEADERS Maths/*.h)
install(
  FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/Maths/
)

# Specify where the library is and where to find the headers
set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
    IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/lib${CMAKE_PROJECT_NAME}.a"
)

install(FILES ${CMAKE_BINARY_DIR}/lib${CMAKE_PROJECT_NAME}.a
    DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(NOT DEFINED DISABLE_PACKAGE_CONFIGURATION)
  configure_package_config_file("Config.cmake.in"
      "${CMAKE_PROJECT_NAME}Config.cmake"
      INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
      PATH_VARS CMAKE_INSTALL_LIBDIR
  )
endif()

#write_basic_package_version_file(
#	${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
#	VERSION 1.0.0
#	COMPATIBILITY SameMajorVersion
#)

### Install Config and ConfigVersion files
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake"
#          "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}"
)

export(PACKAGE ${CMAKE_PROJECT_NAME})

