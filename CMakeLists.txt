cmake_minimum_required(VERSION 3.13)

set (CMAKE_CXX_STANDARD 11)

include(pico-sdk/pico_sdk_init.cmake)

project(OLED LANGUAGES C CXX)

pico_sdk_init()

add_executable(OLED
    main.cpp
    ssd1351.c
    RFID.cpp
    shodan.cpp

)

target_link_libraries(OLED pico_stdlib hardware_spi hardware_dma hardware_i2c)

pico_enable_stdio_usb(OLED 1)
pico_enable_stdio_uart(OLED 0)

pico_add_extra_outputs(OLED)
