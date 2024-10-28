cmake_minimum_required(VERSION 3.0...3.15)

project(GurkaInjector)

include (CBuildKit)

option(USE_STATIC_LINKING "Use static linking for the runtime" ON)

if (USE_STATIC_LINKING)
    if (MSVC)
        # MSVC specific static linking flag
        add_compile_options(/MT$<$<CONFIG:Debug>:d>)
    else()
        # GCC/Clang specific static linking flag
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    endif()
endif()

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

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION include)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/GurkaInjectorConfig.cmake
    DESTINATION lib/cmake/gurka)
