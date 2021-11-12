cmake_minimum_required(VERSION 3.19)
project(21f-pa03-how-bad-can-it-be)

set(CMAKE_CXX_STANDARD 14)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_INCLUDE_DIR /usr/local/src/boost_1_46_1)
set(Boost_LIBRARY_DIR /usr/local/src/boost_1_46_1/stage/lib)

find_package(Boost COMPONENTS system filesystem graph REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    link_directories(${Boost_LIBRARY_DIR})
    add_executable(21f-pa03-how-bad-can-it-be src/main.cpp)
    target_link_libraries(21f-pa03-how-bad-can-it-be  ${Boost_LIBRARIES} )

endif()
