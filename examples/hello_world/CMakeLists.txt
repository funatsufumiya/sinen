cmake_minimum_required(VERSION 3.7)
project(examples)
set(CMAKE_CXX_FLAGS "-std=c++20")

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=native")
endif()

# Hello World Example
file(GLOB HELLO_WORLD_SOURCE_FILES
    ./native/*.cpp
)

include_directories(
    ../include
    ../include/sinen
    ../libs/sol2/include
)

link_directories(..)
find_package(Vulkan REQUIRED)

set(COMMON_LIBRARIES
    sinen
    script_engine
    ${Vulkan_LIBRARIES}    
)

if(WIN32)
    set(LIBRARIES ${COMMON_LIBRARIES})
else()
    set(LIBRARIES ${COMMON_LIBRARIES} ${CMAKE_DL_LIBS})
endif()

add_executable(hello_world
    ${HELLO_WORLD_SOURCE_FILES}
)

target_link_libraries(hello_world 
    ${LIBRARIES}
)