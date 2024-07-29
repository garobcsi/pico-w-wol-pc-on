cmake_minimum_required(VERSION 3.28)

set(PICO_SDK_FETCH_FROM_GIT on)

include(pico_sdk_import.cmake)

set(PICO_PLATFORM rp2040)
set(PICO_BOARD pico_w CACHE STRING "Board type")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE "Release")

project(pico_w_wol_pc_on CXX C ASM)

pico_sdk_init()

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
)

target_link_libraries(${PROJECT_NAME} pico_stdlib
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
