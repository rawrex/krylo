# ZMK Module for [krylo](https://github.com/rawrex/krylo)

A [ZMK](https://zmk.dev/) module for the **krylo** — a 32-key column-staggered split keyboard. Build it as an [external module](https://zmk.dev/docs/development/local-toolchain/build-flash#building-with-external-modules).

The keymap is a lean, self-contained rewrite of [miryoku_zmk](https://github.com/manna-harbour/miryoku), adapted to the krylo's 32 keys.

## Layout

Base layer (Colemak-DH):

```
Q W F P B      J L U Y '
A R S T G      M N E I O
Z X C D        H , . /
    SPACE TAB  RET BSPC
```

- **Home row mods** on `A/R/S/T` (hold → GUI/Alt/Ctrl/Shift) and `N/E/I/O` (Shift/Ctrl/Alt/GUI).
- **Layer-tap thumbs**: `SPACE`→Nav, `TAB`/`RET`→Num, `BSPC`→Sym (hold for layer, tap for key).

## Combos

| Combo | Keys | Output |
|---|---|---|
| `combo_v` | `C` + `D` | `V` |
| `combo_k` | `H` + `,` | `K` |
| `combo_esc` | inner home row (`G` + `M`) | `ESC` |
| `combo_del` | inner thumbs (TAB + RET) | `DEL` |

All base-layer only. `V`/`K` use a 50 ms window; `ESC` uses 85 ms; `DEL` uses 85 ms with `require-prior-idle-ms` to avoid accidental triggers.

## Layers

| Layer | Purpose |
|---|---|
| **Sym** | Symbols and brackets; `to_base_alt_shift` / `to_extra_alt_shift` (Alt+Shift IME toggle); `u_caps_word`; soft off |
| **Num** | Digits and math symbols; battery check (`batt`) |
| **Nav** | Navigation, clipboard (copy/cut/paste/undo/redo), layer toggles |
| **Media** | Media keys and volume; Bluetooth profile selection (`bt_sel_0..3`) |
| **Fun** | F1–F12 and system keys |
| **Cyrillic** | Full Russian ЙЦУКЕН layout (incl. home row mods) |

## Features

- **Double-tap layer guards** (`u_to_U_*`): double-tap a key to switch layers, single tap does nothing.
- **Double-tap bootloader** (`u_bootloader`) instead of a plain reset binding.
- **Soft off** (`soft_off`) on thumb clusters, hold ~2.5 s to power down.
- **Caps word** (`u_caps_word`); hold Shift for regular Caps Lock.
- **Bluetooth profiles 0–3**; double-tap a profile to also clear it. Each select first hops through the unused profile 4 to force a re-advertisement (a plain `BT_SEL` on the already-active profile is a no-op).
- **Clipboard macros** (copy/cut/paste/undo/redo) on the Nav layer — switch between CUA / macOS / Windows variants via `KRYLO_CLIPBOARD_*` toggles in `keymap/config.h`.
- **Battery LED indication** (`batt`): blinks the board LED per charge stage.
- **Language toggle** via Alt+Shift macros on the Sym layer.

## Config toggles (`boards/shields/krylo/keymap/config.h`)

| Toggle | Effect |
|---|---|
| `KRYLO_CLIPBOARD_FUN` | Clipboard via media keys |
| `KRYLO_CLIPBOARD_MAC` | Clipboard via Cmd+Z/C/V/X |
| `KRYLO_CLIPBOARD_WIN` | Clipboard via Ctrl+Z/C/V/X |
