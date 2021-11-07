cmake_minimum_required(VERSION 3.19)
project(boost2)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_INCLUDE_DIR /usr/local/src/boost_1_46_1)
set(Boost_LIBRARY_DIR /usr/local/src/boost_1_46_1/stage/lib)

find_package(Boost COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIR})
    add_executable(boost2 main.cpp)
    target_link_libraries(boost2  ${Boost_LIBRARIES} )
endif()
