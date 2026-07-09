# krylo — AGENTS.md

34-key column-staggered choc split keyboard — a KiCad v10.0 hardware project.  
This repo: PCB, footprints, and symbols only.  
**ZMK firmware is NOT in this repo** — no ZMK config, keymaps, or firmware here.

## Codebase map

| Path | What |
|---|---|
| `PCB/krylo.kicad_sch` | Schematic (rev 0.1) |
| `PCB/krylo.kicad_pcb` | PCB layout, 2-layer, 1.6 mm |
| `PCB/krylo.kicad_pro` | Project settings (DRC rules, net classes, BOM config) |
| `PCB/krylo.pretty/` | **9 custom footprints** (8 functional + 1 artwork) + **1 symbol library** (`krylo.kicad_sym`). Case-sensitive — dir name is `krylo.pretty`, not `krylo.pretty` |
| `PCB/fp-lib-table` | Footprint library table — **exactly ONE entry**: `krylo` → `${KIPRJMOD}/krylo.pretty` |
| `PCB/sym-lib-table` | Symbol library table — references `krylo.kicad_sym` inside `krylo.pretty/` |

## Library gotchas

- **`fp-lib-table` must have exactly one entry.** Two entries (or case-mismatched paths) causes the footprint editor to show nothing.
- **Directory is `krylo.pretty` (capital P).** KiCad gets confused if paths don't match. If renamed, update both `fp-lib-table` and `sym-lib-table`.
- **`fp-info-cache` is auto-generated** and gets stale (orphaned footprint entries). Delete it to force regeneration.
- **`krylo.kicad_prl`** — auto-generated KiCad UI state (layer visibility, window positions). Not for manual editing.
- **`~*.lck` files** in `PCB/` — KiCad lock files, gitignored by pattern `PCB/~*.lck`.
- **`.bak` files** in `krylo.pretty/` — remove them, they don't belong in the library directory.
- **Symbol and footprint names must match** — every symbol in `krylo.kicad_sym` references a footprint in `krylo.pretty/` by name. Renames must be kept in sync.

## Custom footprints (9)

All in `krylo.pretty/` as `.kicad_mod`. **Symbol-to-footprint mapping is by identical name** for all 8 functional footprints.

| Footprint | Used for |
|---|---|
| `D_SOD123` | 1N4148W SMD diodes (16 per board, bottom side), pads with holes are intentional |
| `MOLEX_C` | Molex 781710002 2-pin battery connector (optional, requires jumper bridge) |
| `Jumper` | Solder jumper pads (JP1–JP10) |
| `Mount` | M2 SMT mounting holes (4 per board) |
| `MSK12C02` | Power switch |
| `nice_nano` | MCU module |
| `ResetSW` | Tactile reset button |
| `Switch` | Kailh Choc PG1350 keyswitch sockets (16 per board) |
| `artwork` | PCB silkscreen artwork |

**Naming is consistent:** every symbol's `Footprint` property and `ki_fp_filters` match its footprint file name. (e.g. `krylo:Switch` symbol ↔ `krylo:Switch` footprint). No cross-referencing needed.

Plus 8 `.step` 3D model files in `krylo.pretty/`:

| File | Attached to |
|---|---|
| `nice_nano.step`, `MSK12C02.step` | Matching footprint |
| `switch.step` | `Switch` (lowercase variant, resolves on case-insensitive systems) |
| `keycap.step`, `hot_swap_socket.step` | Extra models on `Switch` footprint |
| `molex_connector.step` | `MOLEX_C` footprint |
| `reset_button.step` | `ResetSW` footprint |
| `smt_m2_nut.step` | `Mount` footprint |

## Custom symbols

8 unique symbol types in `krylo.kicad_sym` (16 definitions including unit variants):

| Symbol | Used for |
|---|---|
| `Jumper` | Solder jumper pads (JP1–JP10) |
| `nice_nano` | MCU module |
| `Switch` | Kailh Choc keyswitch sockets (16 per board) |
| `D_SOD123` | 1N4148W SMD diodes (16 per board) |
| `Mount` | M2 SMT mounting holes (4 per board) |
| `ResetSW` | Tactile reset button |
| `MSK12C02` | Power switch |
| `MOLEX_C` | Molex 781710002 2-pin battery connector (optional, requires jumper bridge) |

**Symbol-to-footprint mapping is by identical name** — all 8 symbols follow this rule. No cross-referencing needed.

## Hardware & manufacturing

- **MCU:** nice!nano (nRF52840); alternatives with compatible pinout work (e.g. Puchi-BLE)
- **Universal PCB for both halves** — same PCB built twice. Side selected by ZMK firmware at runtime (USB-connected half = central)
- **Solder jumper pads (JP1–JP10):** JP1–JP4 are battery mode selection (see below); JP5–JP10 configure power and reset GPIO pins for each side of the split.
  - JST connector mode: bridge JP2+JP3 (top) and JP1+JP4 (bottom)
  - Direct-solder mode: leave open, solder battery wires to PAD1/PAD2
- **Gerber export:** Plot from KiCad to `krylo_0-2_gerbers.zip` (not tracked — regenerate)
- **JLCPCB key specs:** 120.5 × 85.5 mm, 2 layers, 1.6 mm, black, HASL, no impedance control, **remove order number**

## KiCad conventions

- **Only one footprint library** (`krylo`). All footprints are self-contained in `krylo.pretty/`. D_SOD123 references the system 3D model (`${KICAD10_3DMODEL_DIR}/Diode_SMD.3dshapes/D_SOD-123.step`); all other models (including `MOLEX_C`) resolve locally from `krylo.pretty/`.
- **DRC:** min track 0.127 mm, min clearance 0.0 mm, min copper-to-edge 0.5 mm. Violations are `error` severity.
- **Default net class:** track 0.25 mm, via 0.6/0.3 mm, clearance 0.127 mm.

## Field sync workflow

KiCad has a three-layer field system. Fields (Description, Datasheet, etc.) exist at **symbol library**, **footprint library**, and **instance** levels. To avoid back-and-forth diffs, all three must stay in sync.

### Authoritative sources

| Layer | File | Editable directly? |
|---|---|---|
| **Symbol library** (authoritative) | `krylo.pretty/krylo.kicad_sym` | ✅ Safe to edit (text or Symbol Editor) |
| **Footprint library** | `krylo.pretty/*.kicad_mod` | ✅ Safe to edit (text or Footprint Editor) |
| **Embedded symbol cache** | Inside `krylo.kicad_sch` (the `lib_symbols` block) | ❌ **Do not edit directly** — KiCad overwrites from `krylo.kicad_sym` on save |
| **Component instances** | Inside `krylo.kicad_sch` (placed symbols) | ❌ **Do not edit directly** — KiCad overwrites from cache |
| **PCB instances** | Inside `krylo.kicad_pcb` (placed footprints) | ❌ **Do not edit directly** — KiCad overwrites on forward-annotate or save |

### Correct workflow for changing symbol fields (Description, Datasheet)

```
  1. Edit ───────────► krylo.pretty/krylo.kicad_sym
     (KiCad Symbol Editor or safe text edit — this is the source of truth)

  2. Sync schematic ─► Tools → Update Symbols from Library
     (in schematic editor — updates embedded cache + all instances)

  3. Sync PCB ───────► Tools → Update PCB from Schematic (F8)
     (in PCB editor — pushes field values to footprint instances)
```

### Correct workflow for changing footprint fields

```
  1. Edit ───────────► krylo.pretty/*.kicad_mod
     (KiCad Footprint Editor or safe text edit)

  2. Sync PCB ───────► Tools → Update Footprints from Library
     (in PCB editor — syncs instance fields from library)
```

### Rules of thumb

- **Text edits are safe** for `.kicad_sym` and `.kicad_mod` files — KiCad reads these fresh from disk.
- **Text edits are NOT safe** for `.kicad_sch` or `.kicad_pcb` — KiCad holds state in memory and overwrites on save.
- After changing a symbol in `krylo.kicad_sym`, always run **Update Symbols from Library** in the schematic editor, then **Update PCB from Schematic** in the PCB editor.
- When `krylo.kicad_sch` is re-saved by KiCad, any symbol fields that differ between the embedded cache and the external `krylo.kicad_sym` will be **overwritten with the external library's values**. Keep the external library as the single source of truth.

## What not to modify

- `PCB/.history/`, `PCB/~*.lck`, `PCB/krylo.round-tracks-config` — auto-save and lock files (gitignored)
- `fp-info-cache`, `krylo.kicad_prl` — auto-generated; delete to regenerate
- `*.bak` in `krylo.pretty/` — remove if they appear

## No CI / no build scripts

Pure KiCad hardware repo. No package manager, no test runner, no CI.  
