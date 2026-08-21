### ZMK Module for [krylo](https://github.com/rawrex/krylo)
- For how to use refer to the ZMK docs on how to [build with external modules](https://zmk.dev/docs/development/local-toolchain/build-flash#building-with-external-modules)

### The keymap is derived from, [miryoku_zmk](https://github.com/manna-harbour/miryoku)
It is functionally equivalent to the source, with exception of few deliberately omitted features:
- RGB underglow behaviors and keys
- External power (`ext_power`) behaviors
- Mouse keys (move / scroll / buttons), the Mouse layer, and the Button layer
- Output toggle (`u_out_tog`)
- Configurable `U_BOOT` / soft-off / double-tap-boot (macros and `MIRYOKU_KLUDGE_*` toggles) are replaced by direct key bindings below
