// SPDX-License-Identifier: MIT

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/settings/settings.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/keymap.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define KRYLO_LANG_SETTINGS_KEY "krylo/lang"

// Latest Cyrillic layer state to persist
// written to flash by the deferred work item so the keypress path is never blocked by a synchronous flash write
static bool lang_state;

static struct k_work_delayable save_work;

static void save_lang(struct k_work *work)
{
    int ret = settings_save_one(KRYLO_LANG_SETTINGS_KEY, &lang_state, sizeof(lang_state));
    if (ret)
    {
        LOG_ERR("Failed to save language layer state (%d)", ret);
    }
}

static int krylo_lang_init(void)
{
    k_work_init_delayable(&save_work, save_lang);
    return 0;
}

SYS_INIT(krylo_lang_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);

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

    if (cyr_active)
    {
        // The layer-state event raised here makes our listener re-save the same value once after boot
        zmk_keymap_layer_activate(CONFIG_KRYLO_LANG_CYR_LAYER, true);
    }
    return 0;
}

SETTINGS_STATIC_HANDLER_DEFINE(krylo_lang, "krylo", NULL, krylo_lang_settings_set, NULL, NULL);

static int krylo_lang_listener(const zmk_event_t *eh)
{
    const struct zmk_layer_state_changed *ev = as_zmk_layer_state_changed(eh);
    if (!ev || ev->layer != CONFIG_KRYLO_LANG_CYR_LAYER)
    {
        return ZMK_EV_EVENT_BUBBLE;
    }

    lang_state = ev->state;
    k_work_reschedule(&save_work, K_MSEC(CONFIG_KRYLO_LANG_SAVE_DEBOUNCE));
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(krylo_lang, krylo_lang_listener);
ZMK_SUBSCRIPTION(krylo_lang, zmk_layer_state_changed);
