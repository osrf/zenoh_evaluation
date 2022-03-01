cmake_minimum_required(VERSION 3.0)

project(fastdds_montblanc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

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
add_library(datatypes_lib SHARED
  ./src/types/datatypes.cxx
  ./src/types/datatypesPubSubTypes.cxx)
target_link_libraries(datatypes_lib fastcdr fastrtps)

add_library(montblanc_node SHARED ./src/Node.cpp)
target_link_libraries(montblanc_node fastcdr fastrtps)

# Executables
add_executable(main src/main.cpp)
target_link_libraries(main
  fastcdr
  fastrtps
  datatypes_lib
  montblanc_node)

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

# add_executable(Header
#   src/HeaderPublisher.cxx
#   src/HeaderSubscriber.cxx
#   src/HeaderPubSubMain.cxx)
# target_link_libraries(Header
#   fastcdr
#   fastrtps
#   msg_lib)
# # install(TARGETS msg_lib)
# install(TARGETS MSG_TYPES
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#
# # target_link_libraries(test lib)
