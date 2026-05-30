#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int our_driver_set_counter(const struct device *dev,
                           uint32_t value);

#ifdef __cplusplus
}
#endif

#endif