cmake_minimum_required(VERSION 3.20.0)

list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(my_zephyr_app)

if(CONFIG_LED_SENSOR)
	add_subdirectory(drivers/subsys/led_sensor)
endif()

target_sources(app PRIVATE src/main.cpp)
