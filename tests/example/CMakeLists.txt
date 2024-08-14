cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ExampleTest)

set(APP_CORE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../app)

# Connect sources
target_sources(app PRIVATE ${APP_CORE_DIR}/src/example/adder.c)
target_sources(app PRIVATE src/example_test.c)
target_include_directories(app PRIVATE .)
target_include_directories(app PRIVATE ${APP_CORE_DIR}/src/)