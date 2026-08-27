// SPDX-License-Identifier: MIT

#define DT_DRV_COMPAT krylo_led_battery

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led.h>
#include <zephyr/logging/log.h>

#include <drivers/behavior.h>

#include <zmk/behavior.h>
#include <zmk/battery.h>
#include <zmk/activity.h>
#include <zmk/event_manager.h>
#include <zmk/events/activity_state_changed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

struct led_battery_config
{
    size_t leds_len;
    const struct led_dt_spec *leds;

    uint8_t brightness;
    uint32_t on_ms;
    uint32_t off_ms;

    uint8_t high_threshold;
    uint8_t high_count;
    uint8_t mid_threshold;
    uint8_t mid_count;
    uint8_t low_count;
};

struct led_battery_data
{
    struct k_work_delayable work;
    const struct led_battery_config *config;

    uint8_t blinks_left;
    bool phase_on;
};

static void set_leds(const struct led_battery_config *config, uint8_t brightness)
{
    for (size_t i = 0; i < config->leds_len; i++)
    {
        const int err = led_set_brightness_dt(&config->leds[i], brightness);
        if (err)
        {
            LOG_ERR("Failed to set LED %u to %u%%: %d", config->leds[i].index, brightness, err);
        }
    }
}

static void blink_work(struct k_work *work)
{
    struct k_work_delayable *delayable = k_work_delayable_from_work(work);
    struct led_battery_data *data = CONTAINER_OF(delayable, struct led_battery_data, work);
    const struct led_battery_config *config = data->config;

    if (data->blinks_left == 0) 
    {
        set_leds(config, 0);
        return;
    }
    if (data->phase_on) // End of a blink
    {
        set_leds(config, 0);
        data->phase_on = false;
        if (--data->blinks_left > 0) 
        {
            k_work_schedule(&data->work, K_MSEC(config->off_ms));
        }
    }
    else // Start of a blink
    {
        set_leds(config, config->brightness);
        data->phase_on = true;
        k_work_schedule(&data->work, K_MSEC(config->on_ms));
    }
}

static void start_pattern(struct led_battery_data *data, uint8_t blinks) 
{
    data->blinks_left = blinks;
    data->phase_on = false;
    k_work_schedule(&data->work, K_NO_WAIT);
}

static void stop_pattern(const struct device *dev) 
{
    struct led_battery_data *data = dev->data;
    k_work_cancel_delayable(&data->work);
    data->blinks_left = 0;
    set_leds(dev->config, 0);
}

static int led_battery_binding_pressed(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) 
{
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    struct led_battery_data *data = dev->data;
    const struct led_battery_config *config = dev->config;

    const uint8_t soc = zmk_battery_state_of_charge();
    uint8_t blinks;

    if (soc > config->high_threshold) 
    {
        blinks = config->high_count;
    }
    else if (soc > config->mid_threshold) 
    {
        blinks = config->mid_count;
    }
    else 
    {
        blinks = config->low_count;
    }

    LOG_INF("Battery %u%% -> %u blinks", soc, blinks);
    start_pattern(data, blinks);

    return ZMK_BEHAVIOR_OPAQUE;
}

static int led_battery_binding_released(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) 
{
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api led_battery_driver_api = 
{
    .binding_pressed = led_battery_binding_pressed,
    .binding_released = led_battery_binding_released,
    .locality = BEHAVIOR_LOCALITY_EVENT_SOURCE,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

#define LED_DT_SPEC_GET_BY_IDX(node_id, prop, idx) \
LED_DT_SPEC_GET(DT_PHANDLE_BY_IDX(node_id, prop, idx))

#define LEDS_ARRAY(n) DT_CAT(led_battery_leds_, n)

#define DEFINE_LEDS(n) \
static const struct led_dt_spec LEDS_ARRAY(n)[] = \
{ \
    DT_FOREACH_PROP_ELEM_SEP(DT_DRV_INST(n), leds, LED_DT_SPEC_GET_BY_IDX, (, )), \
};

#define LED_BATTERY_INST(n)                                                \
DEFINE_LEDS(n)                                                             \
static const struct led_battery_config led_battery_config_##n =            \
{                                                                          \
.leds_len = ARRAY_SIZE(LEDS_ARRAY(n)),                                     \
    .leds = LEDS_ARRAY(n),                                                 \
    .brightness = DT_INST_PROP_OR(n, brightness, 100),                     \
    .on_ms = DT_INST_PROP_OR(n, on_ms, 200),                               \
    .off_ms = DT_INST_PROP_OR(n, off_ms, 200),                             \
    .high_threshold = DT_INST_PROP_OR(n, high_threshold, 66),              \
    .high_count = DT_INST_PROP_OR(n, high_count, 3),                       \
    .mid_threshold = DT_INST_PROP_OR(n, mid_threshold, 33),                \
    .mid_count = DT_INST_PROP_OR(n, mid_count, 2),                         \
    .low_count = DT_INST_PROP_OR(n, low_count, 1),                         \
};                                                                         \
static struct led_battery_data led_battery_data_##n = {};                  \
BEHAVIOR_DT_INST_DEFINE(n, led_battery_init, NULL, &led_battery_data_##n,  \
&led_battery_config_##n, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, \
&led_battery_driver_api);

#define INST_DEV(n) DEVICE_DT_INST_GET(n),
static const struct device *all_instances[] = {DT_INST_FOREACH_STATUS_OKAY(INST_DEV)};

static void led_battery_disable_all(void) 
{
    for (size_t i = 0; i < ARRAY_SIZE(all_instances); i++) 
    {
        if (device_is_ready(all_instances[i])) 
        {
            stop_pattern(all_instances[i]);
        }
    }
}

static int led_battery_event_listener(const zmk_event_t *eh) 
{
    const struct zmk_activity_state_changed *event = as_zmk_activity_state_changed(eh);
    if (event && event->state != ZMK_ACTIVITY_ACTIVE) // Never leave the LED lit when idle/sleeping on battery power
    {
        led_battery_disable_all();
    }
    return ZMK_EV_EVENT_BUBBLE;
}

static int led_battery_init(const struct device *dev) 
{
    struct led_battery_data *data = dev->data;
    data->config = dev->config;
    k_work_init_delayable(&data->work, blink_work);
    return 0;
}

ZMK_LISTENER(led_battery, led_battery_event_listener);
ZMK_SUBSCRIPTION(led_battery, zmk_activity_state_changed);

DT_INST_FOREACH_STATUS_OKAY(LED_BATTERY_INST)
