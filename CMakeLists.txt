cmake_minimum_required(VERSION 3.20.0)

# Path to out-of-tree boards definitions
set (BOARD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

# Add our custom Zephyr module for drivers w/ syscalls, etc.
list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/drivers/zephyr)

list(APPEND ZEPHYR_EXTRA_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/drivers
)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(grblPANEL)

FILE(GLOB app_sources src/*.c*)
target_sources(app PRIVATE ${app_sources})
target_include_directories(app PRIVATE src)
