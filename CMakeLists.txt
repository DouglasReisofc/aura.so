cmake_minimum_required(VERSION 3.18.1)
project(aura_tsm_rebuild)

set(CMAKE_BUILD_TYPE Release)

add_library(
    TSMRebuild
    SHARED
    src/TSMRebuildMod.cpp
)

target_include_directories(
    TSMRebuild
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(TSMRebuild PUBLIC $<$<COMPILE_LANGUAGE:CXX>:-Werror -std=c++17 -Wall>)
target_compile_options(TSMRebuild PUBLIC -w -Wno-error=format-security -fpermissive -fexceptions)

target_link_libraries(
    TSMRebuild
    log
)

set_target_properties(TSMRebuild PROPERTIES OUTPUT_NAME "TSMRebuild")
