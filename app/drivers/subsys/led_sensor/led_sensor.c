#define DT_DRV_COMPAT zephyr_course_led_sensor

#include <led_sensor.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(led_sensor, CONFIG_SENSOR_LOG_LEVEL);

struct led_sensor_config {
	struct gpio_dt_spec led_gpio;
	const struct led_sensor_driver_api *api;
};

struct led_sensor_data {
	int sample;
};

static int led_sensor_set_sample_impl(const struct device *dev, int sample)
{
	struct led_sensor_data *data = dev->data;

	data->sample = sample;

	return 0;
}

static int led_sensor_get_sample_impl(const struct device *dev)
{
	struct led_sensor_data *data = dev->data;

	return data->sample;
}

static const struct led_sensor_driver_api led_sensor_driver_api = {
	.set_sample = led_sensor_set_sample_impl,
	.get_sample = led_sensor_get_sample_impl,
};

int led_sensor_set_sample(const struct device *dev, int sample)
{
	const struct led_sensor_config *cfg = dev->config;

	return cfg->api->set_sample(dev, sample);
}

int led_sensor_get_sample(const struct device *dev)
{
	const struct led_sensor_config *cfg = dev->config;

	return cfg->api->get_sample(dev);
}

static int led_sensor_init(const struct device *dev)
{
	const struct led_sensor_config *cfg = dev->config;
	int ret;

	if (!gpio_is_ready_dt(&cfg->led_gpio)) {
		LOG_ERR("GPIO device not ready");
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&cfg->led_gpio, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		LOG_ERR("Failed to configure LED GPIO: %d", ret);
		return ret;
	}

#if IS_ENABLED(CONFIG_LED_SENSOR_INIT_STATE)
	ret = gpio_pin_set_dt(&cfg->led_gpio, 1);
	if (ret < 0) {
		return ret;
	}
#endif

	return 0;
}

static int led_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	const struct led_sensor_config *cfg = dev->config;
	int ret;

	if (chan != SENSOR_CHAN_ALL && chan != SENSOR_CHAN_PROX) {
		return -ENOTSUP;
	}

	ret = gpio_pin_set_dt(&cfg->led_gpio, 1);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

static int led_sensor_channel_get(const struct device *dev, enum sensor_channel chan,
				  struct sensor_value *val)
{
	const struct led_sensor_config *cfg = dev->config;
	struct led_sensor_data *data = dev->data;
	int ret;

	if (chan != SENSOR_CHAN_PROX) {
		return -ENOTSUP;
	}

	ret = gpio_pin_set_dt(&cfg->led_gpio, 0);
	if (ret < 0) {
		return ret;
	}

	val->val1 = data->sample;
	val->val2 = 0;

	return 0;
}

static DEVICE_API(sensor, led_sensor_sensor_api) = {
	.sample_fetch = led_sensor_sample_fetch,
	.channel_get = led_sensor_channel_get,
};

#define LED_SENSOR_DEFINE(inst)							\
	static struct led_sensor_data led_sensor_data_##inst;			\
	static const struct led_sensor_config led_sensor_config_##inst = {	\
		.led_gpio = GPIO_DT_SPEC_INST_GET(inst, gpios),			\
		.api = &led_sensor_driver_api,					\
	};									\
	SENSOR_DEVICE_DT_INST_DEFINE(inst, led_sensor_init, NULL,		\
				     &led_sensor_data_##inst,			\
				     &led_sensor_config_##inst, POST_KERNEL,	\
				     CONFIG_SENSOR_INIT_PRIORITY,		\
				     &led_sensor_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_DEFINE)
