cmake_minimum_required(VERSION 3.0)

project(fastdds_montblanc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

#Check C++11
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++11 SUPPORTS_CXX11)
    if(NOT SUPPORTS_CXX11)
        message(FATAL_ERROR "Compiler doesn't support C++11")
    endif()
endif()

include_directories(
  ${PROJECT_SOURCE_DIR}/include/
)

# Libs
# The following is really hacky but...
include(ExternalProject)
ExternalProject_Add(fastdds_montblanc_types
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/../dds
  BINARY_DIR ${CMAKE_BINARY_DIR}/dds-build
  STEP_TARGETS build
  EXCLUDE_FROM_ALL TRUE
  INSTALL_COMMAND cmake -E echo "Skipping install step.")

add_library(datatypes_lib UNKNOWN IMPORTED)
set_target_properties(datatypes_lib PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/dds-build/lib/libdatatypes_lib.so)
include_directories(${CMAKE_SOURCE_DIR}/../dds/include)
add_dependencies(datatypes_lib fastdds_montblanc_types)

add_library(montblanc_node SHARED ./src/Node.cpp)
target_link_libraries(montblanc_node fastcdr fastrtps)

add_library(montblanc_utils INTERFACE)
target_include_directories(montblanc_utils INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
  target_link_libraries(montblanc_utils INTERFACE fastcdr fastrtps)

# Executables
file(GLOB_RECURSE node_files RELATIVE ${CMAKE_SOURCE_DIR} "src/nodes/*.cpp")

foreach(file_path ${node_files})
  string( REPLACE ".cpp" "" new_name ${file_path} )
  get_filename_component(filename ${new_name} NAME)
  add_executable( ${filename} ${file_path} )
  target_link_libraries(${filename}
    fastcdr
    fastrtps
    datatypes_lib
    montblanc_node)
  set_target_properties(${filename}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nodes")
endforeach()
