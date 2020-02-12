cmake_minimum_required(VERSION 2.8.3)

# Configure CCache if available
find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif (CCACHE_FOUND)

option(WITH_COVERAGE "Compile with code coverage" OFF)
if (WITH_COVERAGE)
  message(STATUS "Compiling with code coverage support.")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
endif ()

add_definitions("-std=c++11")

# -----------------------
# Project initialization
# -----------------------
project(phenom_simulator)
set(${PROJECT_NAME}_VERSION 1.0.0)

option(BUILD_TESTS "Use GTest to build and test libraries" ON)

# all wet packages need to be listed
find_package(catkin REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(PkgConfig REQUIRED)

# declare the package
catkin_package(INCLUDE_DIRS src
  LIBRARIES ${PROJECT_NAME})

# declare directories
include_directories(src
  ${Boost_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  )

# add code libraries
add_library(lorenz_lib
  src/lorenz/lorenz.cc)
set_target_properties(lorenz_lib PROPERTIES NO_SYSTEM_FROM_IMPORTED TRUE)
target_compile_options(lorenz_lib PUBLIC ${${PROJECT_NAME}_COMPILER_FLAGS})
target_link_libraries(lorenz_lib ${catkin_LIBRARIES})

add_library(ad_lib
  src/ad_pde/ad_1d.cc)
set_target_properties(ad_lib PROPERTIES NO_SYSTEM_FROM_IMPORTED TRUE)
target_compile_options(ad_lib PUBLIC ${${PROJECT_NAME}_COMPILER_FLAGS})
target_link_libraries(ad_lib ${catkin_LIBRARIES})

# add code executables
add_executable(lorenz_sim
	src/lorenz/lorenz.cc)
add_dependencies(lorenz_sim ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
set_target_properties(lorenz_sim PROPERTIES NO_SYSTEM_FROM_IMPORTED TRUE)
target_compile_options(lorenz_sim PUBLIC ${${PROJECT_NAME}_COMPILER_FLAGS})
target_link_libraries(lorenz_sim ${catkin_LIBRARIES} lorenz_lib)

add_executable(ad_1d_sim
  src/ad_pde/ad_1d.cc)
add_dependencies(ad_1d_sim ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
set_target_properties(ad_1d_sim PROPERTIES NO_SYSTEM_FROM_IMPORTED TRUE)
target_compile_options(ad_1d_sim PUBLIC ${${PROJECT_NAME}_COMPILER_FLAGS})
target_link_libraries(ad_1d_sim ${catkin_LIBRARIES} ad_lib)

#############
## Testing ##
#############

if (BUILD_TESTS)
  enable_testing()
  file(GLOB TEST_SOURCES "test/*.cc")
  add_executable(${PROJECT_NAME}_test ${TEST_SOURCES} ./test/gtest/gtest-all.cc)
  target_link_libraries(${PROJECT_NAME}_test ${catkin_LIBRARIES} pthread)
  set_target_properties(${PROJECT_NAME}_test PROPERTIES COMPILE_FLAGS "-std=c++11 -Wno-deprecated-register")
  add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test --gtest_color=yes)
endif()