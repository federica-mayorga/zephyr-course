if(CONFIG_LED_SENSOR)
	target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/led_sensor.c)
endif()
