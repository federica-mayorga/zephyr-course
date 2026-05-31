#include <led_sensor.h>

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led-sensor" alias. */
#define LED_SENSOR_NODE DT_ALIAS(led_sensor)

static const struct device *const led_sensor = DEVICE_DT_GET(LED_SENSOR_NODE);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	bool led_state = true;
	struct sensor_value val;

	if (!device_is_ready(led_sensor)) {
		return 0;
	}

	while (1) {
		if (led_sensor_set_sample(led_sensor, led_state ? 1 : 0) < 0) {
			return 0;
		}

		if (sensor_sample_fetch_chan(led_sensor, SENSOR_CHAN_ALL) < 0) {
			return 0;
		}

		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS / 2);

		if (sensor_channel_get(led_sensor, SENSOR_CHAN_PROX, &val) < 0) {
			return 0;
		}

		led_state = !led_state;
		LOG_INF("LED state: %s, sample: %d", led_state ? "ON" : "OFF",
			led_sensor_get_sample(led_sensor));

		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS / 2);
	}
	return 0;
}
