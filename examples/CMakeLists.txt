message(STATUS "Examples: CXXFLAGS: ${CMAKE_CXX_FLAGS}")

list(APPEND GUM_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

 
        list(APPEND GUM_LIBRARIES ${PROJECT_NAME})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/)


link_directories(${CMAKE_BINARY_DIR}/src/)

add_definitions(-DEXAMPLES_PATH=\"${CMAKE_SOURCE_DIR}/examples\")

set(EXAMPLE_FILE_LIST ColorConversion Matrices Vectors)

foreach(EXAMPLE ${EXAMPLE_FILE_LIST})
    message(STATUS "Adding target ${EXAMPLE}")
    add_executable(Example_${EXAMPLE} "./${EXAMPLE}.cpp")
    target_link_libraries(Example_${EXAMPLE} "${GUM_LIBRARIES}")
endforeach()
