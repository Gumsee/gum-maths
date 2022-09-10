cmake_minimum_required (VERSION 3.0)
project(GumMaths)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

#Set OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DGUM_OS_LINUX)
    set(GUM_OS_LINUX true)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DGUM_OS_WINDOWS)
    set(GUM_OS_WINDOWS true)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DGUM_OS_MACOS)
    set(GUM_OS_MACOS true)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    add_definitions(-DGUM_OS_BSD)
    set(GUM_OS_BSD true)
endif()



#add_definitions(-DDEBUG)
add_definitions(-DCHECK_GL_ERRORS)



add_subdirectory(src)
add_subdirectory(examples)
