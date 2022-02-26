cmake_minimum_required(VERSION 3.0)

project(fastdds_montblanc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib") 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

include_directories(
  ${PROJECT_SOURCE_DIR}/include/
)

# Libs
add_library(lib SHARED src/lib.cpp)
target_link_libraries(test lib)
