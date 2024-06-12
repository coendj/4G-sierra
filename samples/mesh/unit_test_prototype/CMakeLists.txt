# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
message(STATUS "Zephyr base: $ENV{ZEPHYR_BASE}")
message(STATUS "current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

project(unit_test_prototype)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/mocks)


target_sources(app PRIVATE src/main.c
                src/gsm_driver_mock.c
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/heartbeat_handler.c
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/node.c
)


