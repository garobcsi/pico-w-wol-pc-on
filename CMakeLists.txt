cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PICO_SDK_FETCH_FROM_GIT on)

set(PICO_PLATFORM rp2040)
set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(CMAKE_BUILD_TYPE "Release")

project(pico_w_wol_pc_on C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

pico_sdk_init()

add_executable(${PROJECT_NAME} main.cpp)

pico_set_program_name(${PROJECT_NAME} "WolPcOn")
pico_set_program_version(${PROJECT_NAME} "1.0")

pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 1)

target_link_libraries(${PROJECT_NAME}
        pico_stdlib)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
)

target_link_libraries(${PROJECT_NAME}
        hardware_spi
        hardware_i2c
        hardware_dma
        hardware_pio
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        pico_cyw43_arch_lwip_poll)

pico_add_extra_outputs(${PROJECT_NAME})
