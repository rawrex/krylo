// SPDX-License-Identifier: MIT

#include <zephyr/kernel.h>
#include <zephyr/settings/settings.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/keymap.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define KRYLO_LANG_SETTINGS_KEY "krylo/lang"

// Set while restoring on boot so the resulting layer-state event isn't
// written straight back to flash.
static bool restoring;

static void save_lang(bool cyr_active) 
{
    int ret = settings_save_one(KRYLO_LANG_SETTINGS_KEY, &cyr_active, sizeof(cyr_active));
    if (ret) 
    {
        LOG_ERR("Failed to save language layer state (%d)", ret);
    }
}

static int krylo_lang_settings_set(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg)
{
    const char *next;
    if (!settings_name_steq(name, "lang", &next) || next) 
    {
        return 0; // not our setting
    }

    bool cyr_active;
    if (len != sizeof(cyr_active) || read_cb(cb_arg, &cyr_active, sizeof(cyr_active)) < 0) 
    {
        return -EINVAL;
    }

    restoring = true;
    if (cyr_active) 
    {
        zmk_keymap_layer_activate(CONFIG_KRYLO_LANG_CYR_LAYER, true);
    }
    restoring = false;
    return 0;
}

SETTINGS_STATIC_HANDLER_DEFINE(krylo_lang, "krylo", NULL, krylo_lang_settings_set, NULL, NULL);

static int krylo_lang_listener(const zmk_event_t *eh) 
{
    const struct zmk_layer_state_changed *ev = as_zmk_layer_state_changed(eh);
    if (!ev || ev->layer != CONFIG_KRYLO_LANG_CYR_LAYER || restoring) 
    {
        return ZMK_EV_EVENT_BUBBLE;
    }

    save_lang(ev->state);
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(krylo_lang, krylo_lang_listener);
ZMK_SUBSCRIPTION(krylo_lang, zmk_layer_state_changed);
