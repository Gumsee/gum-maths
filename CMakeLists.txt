cmake_minimum_required (VERSION 3.0)
project (GumMaths)

add_definitions(-DDEBUG)

set(GUM_INSTALLDIR "/usr/include/gumball/")

add_subdirectory(./src/)
add_subdirectory(./examples/)
