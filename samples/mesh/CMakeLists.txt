# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

include("${CMAKE_CURRENT_LIST_DIR}/common/version/Version.txt")

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
message("BOARD_ROOT: ${BOARD_ROOT}")
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(mesh-network)

target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/lora_driver.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gsm_driver.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/heartbeat_handler.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/node.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sram_setup.c
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
