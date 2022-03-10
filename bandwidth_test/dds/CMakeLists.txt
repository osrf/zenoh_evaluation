cmake_minimum_required(VERSION 2.8.12)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(FastddsTester)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT foonathan_memory_FOUND)
    find_package(foonathan_memory REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

include_directories(include)

add_library(datatypes UNKNOWN IMPORTED)
set_target_properties(datatypes PROPERTIES
  IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../../dds_types/build/lib/libdds_types.so)
include_directories(${CMAKE_SOURCE_DIR}/../../dds_types/include)
add_dependencies(datatypes fastdds_montblanc_types)

add_library(dds_node SHARED src/Node.cpp)
target_link_libraries(dds_node fastcdr fastrtps)

add_executable(publisher src/publisher.cpp)
target_compile_definitions(publisher PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG>
)
target_link_libraries(publisher
  fastrtps
  fastcdr
  foonathan_memory
  datatypes
  dds_node)

add_executable(subscriber src/subscriber.cpp)
target_compile_definitions(subscriber PRIVATE
    $<$<AND:$<NOT:$<BOOL:${WIN32}>>,$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">>:__DEBUG>
    $<$<BOOL:${INTERNAL_DEBUG}>:__INTERNALDEBUG>
)
target_link_libraries(subscriber
  fastrtps
  fastcdr
  foonathan_memory
  datatypes
  dds_node)
