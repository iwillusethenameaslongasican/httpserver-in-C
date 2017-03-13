cmake_minimum_required(VERSION 3.6)
project(httpServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")

set(SOURCE_FILES httpd.cpp)
add_executable(httpdServer ${SOURCE_FILES})
find_package(Threads REQUIRED )
target_link_libraries(httpdServer Threads::Threads )