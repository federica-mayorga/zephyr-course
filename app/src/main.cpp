#include <led_sensor.h>

#include <errno.h>
#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/util.h>

#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led-sensor" alias. */
#define LED_SENSOR_NODE DT_ALIAS(led_sensor)

static const struct device *const led_sensor = DEVICE_DT_GET(LED_SENSOR_NODE);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

extern "C" int cmd_sensor_fetch(const struct shell *shell, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (!device_is_ready(led_sensor)) {
		shell_error(shell, "sensor device is not ready");
		return -ENODEV;
	}

	int ret = sensor_sample_fetch(led_sensor);
	if (ret < 0) {
		shell_error(shell, "sensor_sample_fetch() failed: %d", ret);
		return ret;
	}

	shell_print(shell, "sensor_sample_fetch() called");
	return 0;
}

extern "C" int cmd_sensor_read(const struct shell *shell, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);
	struct sensor_value val;
	int ret;

	if (!device_is_ready(led_sensor)) {
		shell_error(shell, "sensor device is not ready");
		return -ENODEV;
	}

	ret = sensor_channel_get(led_sensor, SENSOR_CHAN_PROX, &val);
	if (ret < 0) {
		shell_error(shell, "sensor_channel_get() failed: %d", ret);
		return ret;
	}

	shell_print(shell, "prox: %d.%06d", val.val1, val.val2);
	return 0;
}

extern "C" int cmd_sensor_info(const struct shell *shell, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(shell, "name: %s", led_sensor->name);
	shell_print(shell, "ready: %s", device_is_ready(led_sensor) ? "yes" : "no");
	return 0;
}

extern "C" int cmd_sensor_set(const struct shell *shell, size_t argc, char **argv)
{
	char *end = NULL;
	long value;
	int ret;

	if (!device_is_ready(led_sensor)) {
		shell_error(shell, "sensor device is not ready");
		return -ENODEV;
	}

	if (argc != 2) {
		shell_error(shell, "usage: sensor ser <value>");
		return -EINVAL;
	}

	errno = 0;
	value = strtol(argv[1], &end, 10);
	if (errno != 0 || end == argv[1] || *end != '\0' || value < 0 || value > 1) {
		shell_error(shell, "value must be 0 or 1");
		return -EINVAL;
	}

	ret = led_sensor_set_sample(led_sensor, (int)value);
	if (ret < 0) {
		shell_error(shell, "led_sensor_set_sample() failed: %d", ret);
		return ret;
	}

	shell_print(shell, "sample set to %ld", value);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcmds,
	SHELL_CMD(fetch, NULL, "Call sensor_sample_fetch()", cmd_sensor_fetch),
	SHELL_CMD(read, NULL, "Call sensor_channel_get() for PROX", cmd_sensor_read),
	SHELL_CMD(info, NULL, "Print sensor name and ready state", cmd_sensor_info),
	SHELL_CMD_ARG(ser, NULL, "Set sensor sample value (0 or 1)", cmd_sensor_set, 2, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_subcmds, "LED sensor commands", NULL);

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
		LOG_DBG("LED state: %s, sample: %d", led_state ? "ON" : "OFF",
			led_sensor_get_sample(led_sensor));

		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS / 2);
	}
	return 0;
}
