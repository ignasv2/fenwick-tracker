cmake_minimum_required (VERSION 3.16)

# set default build to debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Release or Debug" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

# get folder name as project name
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

# define cmake variables for file locations
file(GLOB USER_FILES_1
  "code/*.h"
  "code/*.cpp"
)

# create an executable in the app folder
add_executable(run_app "app/main.cpp" ${USER_FILES_1})