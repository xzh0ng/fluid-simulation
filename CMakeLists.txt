cmake_minimum_required(VERSION 2.6)
project(fluid-simulation)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if(APPLE)
  SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall")
else()
  SET(GCC_COVERAGE_COMPILE_FLAGS "-Wa,-mbig-obj")
endif()

OPTION (USE_OpenMP "Use OpenMP" ON)
IF(USE_OpenMP)
  FIND_PACKAGE(OpenMP)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  ENDIF()
ENDIF()


### libIGL options: choose between header only and compiled static library
option(LIBIGL_USE_STATIC_LIBRARY     "Use libigl as static library" OFF)
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_PNG               "Use PNG"            ON)

find_package(LIBIGL REQUIRED)

# Add your project files
include_directories("include/")
if(USE_SOLUTION)
  file(GLOB SRCFILES solution/*.cpp)
else()
  file(GLOB SRCFILES src/*.cpp)
endif()
file(GLOB HEADER include/*.h)

add_library(core ${SRCFILES} ${HEADER})
target_link_libraries(core igl::core igl::opengl igl::opengl_glfw igl::png)

add_executable(${PROJECT_NAME} "main.cpp")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
target_link_libraries(${PROJECT_NAME} core igl::core igl::opengl igl::opengl_glfw igl::png)
