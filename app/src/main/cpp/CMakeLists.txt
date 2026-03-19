cmake_minimum_required(VERSION 3.19.0)
set(CMAKE_BUILD_TYPE Release)
set(ANDROID_PLATFORM 31)
set(CIPHER_PATH ${CMAKE_SOURCE_DIR}/lib/libciphered.so)
if (NOT EXISTS ${CIPHER_PATH})
        message(FATAL_ERROR "Missing ${CIPHER_PATH}. Run scripts/pull_libciphered.ps1 before build.")
endif()

project("userlib")

file(READ ${CMAKE_HOME_DIRECTORY}/config.json MY_JSON_STRING) #reads soname from config
string(JSON CUR_NAME GET ${MY_JSON_STRING} name)
string(LENGTH ${CUR_NAME} CUR_NAME_LEN)
string(SUBSTRING ${CUR_NAME} 3 ${CUR_NAME_LEN} CUR_NAME) #ugly but there's no lazy regex support
string(REPLACE ".so" "" CUR_NAME ${CUR_NAME})
add_compile_options("-fvisibility=hidden")
add_link_options("-s")
add_library( # Sets the name of the library.
        ${CUR_NAME}
        SHARED
        main.cpp
        spiritshop.cpp
        lights.cpp
        contextops.cpp
        invitemanager.cpp
        translation.cpp
        worldquests.cpp
        heartselector.cpp
        changelevel.cpp
        iap_purchase.cpp
        dlfake/fake_dlfcn.c
        ${CMAKE_SOURCE_DIR}/classes_dex.o
        )

add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/classes_dex.o
        COMMAND ${CMAKE_LINKER}
        ARGS -r -b binary -m aarch64elf classes.dex -o ${CMAKE_SOURCE_DIR}/classes_dex.o
        DEPENDS ${CMAKE_SOURCE_DIR}/classes.dex
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating classes_dex.o"
        VERBATIM
        )

set_target_properties(${CUR_NAME}
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )

find_library(
        log-lib
        log)

target_link_libraries(
        ${CUR_NAME}
        "${CIPHER_PATH}"
        ${log-lib}
        android
        )

add_custom_command(
        TARGET ${CUR_NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
        ARGS --add-section .config=${CMAKE_HOME_DIRECTORY}/config.json --set-section-flags .config=readonly,contents,data $<TARGET_FILE:${CUR_NAME}>
)

add_custom_command(
        TARGET ${CUR_NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
        ARGS --add-section .icon=${CMAKE_HOME_DIRECTORY}/aw.webp --set-section-flags .icon=readonly,contents,data $<TARGET_FILE:${CUR_NAME}>
)
