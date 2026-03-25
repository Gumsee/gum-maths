message(STATUS "Tests: CXXFLAGS: ${CMAKE_CXX_FLAGS}")
ENABLE_TESTING()
SET(CTEST_OUTPUT_ON_FAILURE TRUE)
include(CTest)

list(APPEND GUM_LIBRARIES 
  ${CMAKE_THREAD_LIBS_INIT}
  ${CMAKE_PROJECT_NAME}
)

include_directories(${CMAKE_SOURCE_DIR}/src/)
link_directories(${CMAKE_BINARY_DIR}/src/)

set(TEST_FILE_LIST 
  QuaternionConversion
)

foreach(TEST ${TEST_FILE_LIST})
    message(STATUS "Adding target ${TEST}")
    add_executable(Test_${TEST} "./${TEST}.cpp")
    target_link_libraries(Test_${TEST} "${GUM_LIBRARIES}")

    add_test(NAME ${TEST} COMMAND Test_${TEST})
endforeach()
