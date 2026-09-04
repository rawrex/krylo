# SPDX-License-Identifier: MIT

target_sources_ifdef(CONFIG_KRYLO_LED_BATTERY app PRIVATE src/krylo_led_battery.c)
target_sources_ifdef(CONFIG_KRYLO_LANG_PERSIST app PRIVATE src/krylo_lang_persist.c)
