if(CONFIG_LED_SENSOR)
	target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/led_sensor.c)
	target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()
