cmake_minimum_required(VERSION 3.0...3.15)

project(dll-injection-skeleton)

set(CMAKE_CXX_STANDARD 17)
set(LOCAL_PROJECT OFF) 

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	set(LOCAL_PROJECT ON) 
endif()

option(DIS_BUILD_EXAMPLES "Build examples for Dll Injection Skeleton" ${LOCAL_PROJECT})
option(DIS_ENABLE_TESTING "Build examples for Dll Injection Skeleton" ${LOCAL_PROJECT})

add_subdirectory(src)

if(DIS_BUILD_EXAMPLES)
	add_subdirectory(samples)
endif()

if(DIS_ENABLE_TESTING)
	add_subdirectory(tests)
endif()
