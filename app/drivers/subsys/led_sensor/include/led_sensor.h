#pragma once

#include <zephyr/device.h>
#include <zephyr/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Extension API (in addition to the Zephyr sensor driver API). */
__subsystem struct led_sensor_driver_api {
	int (*set_sample)(const struct device *dev, int sample);
	int (*get_sample)(const struct device *dev);
};

int led_sensor_set_sample(const struct device *dev, int sample);
int led_sensor_get_sample(const struct device *dev);

#ifdef __cplusplus
}
#endif
