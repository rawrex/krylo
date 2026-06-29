# Pinky — AGENTS.md

34-key column-staggered choc split keyboard — a KiCad v10.0 hardware project.  
This repo: PCB, footprints, and symbols only.  
**ZMK firmware is NOT in this repo** — no ZMK config, keymaps, or firmware here.

## Codebase map

| Path | What |
|---|---|
| `PCB/pinky.kicad_sch` | Schematic (rev 0.1) |
| `PCB/pinky.kicad_pcb` | PCB layout, 2-layer, 1.6 mm |
| `PCB/pinky.kicad_pro` | Project settings (DRC rules, net classes, BOM config) |
| `PCB/Pinky.pretty/` | **8 custom footprints** + **1 symbol library** (`pinky.kicad_sym`). Case-sensitive — dir name is `Pinky.pretty`, not `pinky.pretty` |
| `PCB/fp-lib-table` | Footprint library table — **exactly ONE entry**: `Pinky` → `${KIPRJMOD}/Pinky.pretty` |
| `PCB/sym-lib-table` | Symbol library table — references `pinky.kicad_sym` inside `Pinky.pretty/` |

## Library gotchas

- **`fp-lib-table` must have exactly one entry.** Two entries (or case-mismatched paths) causes the footprint editor to show nothing.
- **Directory is `Pinky.pretty` (capital P).** KiCad gets confused if paths don't match. If renamed, update both `fp-lib-table` and `sym-lib-table`.
- **`fp-info-cache` is auto-generated** and gets stale (orphaned footprint entries). Delete it to force regeneration.
- **`pinky.kicad_prl`** — auto-generated KiCad UI state (layer visibility, window positions). Not for manual editing.
- **`~*.lck` files** in `PCB/` — KiCad lock files, gitignored by pattern `PCB/~*.lck`.
- **`.bak` files** in `Pinky.pretty/` — remove them, they don't belong in the library directory.
- **Symbol and footprint names must match** — every symbol in `pinky.kicad_sym` references a footprint in `Pinky.pretty/` by name. Renames must be kept in sync.

## Custom footprints (8)

All in `Pinky.pretty/` as `.kicad_mod`:

| Footprint | Used for |
|---|---|
| `D_SOD123` | 1N4148W SMD diodes (17 per board, bottom side) |
| `JST_PH` | Battery connector (optional, requires jumper bridge) |
| `Jumper` | Solder jumper pads (JP1–JP4) |
| `MountingHole` | M2 mounting holes (5 per board) |
| `MSK12C02` | Power switch |
| `nice_nano` | MCU module |
| `ResetSW` | Tactile reset button |
| `Switch` | Kailh Choc PG1350 keyswitch sockets (17 per board) |

**Naming is consistent:** each footprint name matches its file name and its symbol name in `pinky.kicad_sym`.

Plus 6 `.step` 3D model files in `Pinky.pretty/` named by matching footprint name.

## Custom symbols

8 unique symbol types in `pinky.kicad_sym` (16 definitions including unit variants):

| Symbol | Used for |
|---|---|
| `Jumper` | Solder jumper pads (JP1–JP4) |
| `nice_nano` | MCU module |
| `Switch` | Kailh Choc keyswitch sockets (17 per board) |
| `D_SOD123` | 1N4148W SMD diodes (17 per board) |
| `MountingHole` | Mounting holes (5 per board) |
| `ResetSW` | Tactile reset button |
| `MSK12C02` | Power switch |
| `JST_PH` | Battery connector (optional, requires jumper bridge) |

**Symbol-to-footprint mapping is by identical name** — e.g. `Pinky:Switch` (symbol) ↔ `Pinky:Switch` (footprint). No cross-referencing needed.

## Hardware & manufacturing

- **MCU:** nice!nano (nRF52840); alternatives with compatible pinout work (e.g. Puchi-BLE)
- **Universal PCB for both halves** — same PCB built twice. Side selected by ZMK firmware at runtime (USB-connected half = central)
- **Solder jumper pads (JP1–JP4):**
  - JST connector mode: bridge JP2+JP3 (top) and JP1+JP4 (bottom)
  - Direct-solder mode: leave open, solder battery wires to PAD1/PAD2
- **Gerber export:** Plot from KiCad to `Pinky_0-2_gerbers.zip` (not tracked — regenerate)
- **JLCPCB key specs:** 120.5 × 85.5 mm, 2 layers, 1.6 mm, black, HASL, no impedance control, **remove order number**

## KiCad conventions

- **Only one footprint library** (`Pinky`). All footprints are local — no system library dependencies.
- **DRC:** min track 0.127 mm, min clearance 0.0 mm, min copper-to-edge 0.5 mm. Violations are `error` severity.
- **Default net class:** track 0.25 mm, via 0.6/0.3 mm, clearance 0.127 mm.

## What not to modify

- `PCB/.history/`, `PCB/~*.lck`, `PCB/pinky.round-tracks-config` — auto-save and lock files (gitignored)
- `fp-info-cache`, `pinky.kicad_prl` — auto-generated; delete to regenerate
- `*.bak` in `Pinky.pretty/` — remove if they appear

## No CI / no build scripts

Pure KiCad hardware repo. No package manager, no test runner, no CI.  
