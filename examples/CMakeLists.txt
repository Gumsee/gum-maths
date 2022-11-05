set (CMAKE_EXPORT_COMPILE_COMMANDS 1)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS ON)
set (INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/${CMAKE_BUILD_TYPE}/bin/")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g3 -Wall -fdiagnostics-color=always")
if(DEFINED GUM_OS_LINUX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-unused-parameter -Wno-reorder -Wno-pedantic")
elseif(DEFINED GUM_OS_WINDOWS)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /no-Qspectre")
endif()
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0") #Optimization

message(STATUS "Examples: CXXFLAGS: ${CMAKE_CXX_FLAGS}")

list(APPEND GUM_LIBRARIES ${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})



include_directories(${CMAKE_SOURCE_DIR}/src/)


link_directories(${CMAKE_BINARY_DIR}/src/)

add_definitions(-DEXAMPLES_PATH=\"${CMAKE_SOURCE_DIR}/examples\")

set(EXAMPLE_FILE_LIST ColorConversion Matrices Vectors)

foreach(EXAMPLE ${EXAMPLE_FILE_LIST})
    message(STATUS "Adding target ${EXAMPLE}")
    add_executable(Example_${EXAMPLE} "./${EXAMPLE}.cpp")
    target_link_libraries(Example_${EXAMPLE} "${GUM_LIBRARIES}")
endforeach()
