cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME ate-chips)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "-g -Wall -Wpedantic -Werror")

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${COMMON_INCLUDES})
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)

set(CMAKE_CXX_STANDARD 17)


                    #############
                    ### GTEST ###
                    #############
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB PROJECT_HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)

set(PROJECT_SRC main.cpp)
file(GLOB PROJECT_LIB ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB PROJECT_TESTS ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${PROJECT_HEADERS})
add_executable(${PROJECT_NAME}_tests ${PROJECT_TESTS} ${PROJECT_HEADERS})
add_library(${PROJECT_NAME}_lib ${PROJECT_LIB} ${PROJECT_HEADERS})

target_link_libraries(${PROJECT_NAME}_tests gtest_main)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib pthread sfml-graphics sfml-audio)
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME}_lib)

enable_testing()
add_test(NAME test COMMAND ${PROJECT_NAME}_tests)
