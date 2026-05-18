#include "zephyr/device.h"
#include "zephyr/devicetree.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h> 

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_config {
    struct gpio_dt_spec led;
};

static int channel_get_my_impl(const struct device *dev,
				               enum sensor_channel chan,
				               struct sensor_value *val) {

    const struct our_driver_config *config = dev->config;

    gpio_pin_set_dt(&config->led, 0);
    LOG_INF("Channel get called, channel %d", chan);
    LOG_INF("LED OFF");

    return 0;
}

static int sample_fetch_my_impl(const struct device *dev,
                               enum sensor_channel chan) {

    const struct our_driver_config *config = dev->config;

    gpio_pin_set_dt(&config->led, 1);
    LOG_INF("Sample fetch called, channel %d", chan);
    LOG_INF("LED ON");

    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
    .channel_get = channel_get_my_impl,
    .sample_fetch = sample_fetch_my_impl,
};

// Init fn
static int init(const struct device *dev) {
    const struct our_driver_config *cfg = dev->config;

    if (!gpio_is_ready_dt(&cfg->led)) {
        LOG_ERR("LED GPIO not ready");
        return -ENODEV;
    }

    gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);

    LOG_INF("Device Initialized!");

    return 0;
}

#define DEV_INST(inst) \
    static const struct our_driver_config cfg_##inst = {            \
        .led = GPIO_DT_SPEC_INST_GET(inst, led_gpios),              \
    };                                                              \
                                                                    \
    DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, &cfg_##inst, POST_KERNEL, 80, &api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);