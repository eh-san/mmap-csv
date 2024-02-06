cmake_minimum_required(VERSION 3.20)

project(mmap-csv LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(SCRIPT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
set(EXAMPLE_DIRS ${BUILD_DIR}/example)

file(GLOB_RECURSE SOURCES "src/*.h" "src/*.cpp")


add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE src)

add_custom_target(
    with-example
    COMMAND ${PYTHON3} ${SCRIPT_DIRS}/generator.py --count 10 --path ${EXAMPLE_DIRS}
    DEPENDS ${PROJECT_NAME}
)
  
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Werror -Wextra -Wpedantic -Wconversion)
  
endif()
