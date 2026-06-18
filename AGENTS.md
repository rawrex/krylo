# Pinky — AGENTS.md

34-key column-staggered choc split keyboard — a KiCad hardware project.  
This repo: PCB, footprints, and symbols.  
**ZMK firmware is NOT in this repo** — no ZMK config, keymaps, or firmware here.

## Codebase map

| Path | What |
|---|---|
| `PCB/pinky.kicad_sch` | Schematic (KiCad v10.0, format 20260306) |
| `PCB/pinky.kicad_pcb` | PCB layout, 2-layer, 1.6 mm, rev 0.2 |
| `PCB/pinky.kicad_pro` | Project settings (DRC rules, net classes, BOM config) |
| `PCB/Pinky.pretty/` | **7 custom footprints** + **4 symbol library files** (`.kicad_sym`). Case-sensitive — dir name is `Pinky.pretty`, not `pinky.pretty` |
| `PCB/fp-lib-table` | Footprint library table — must have exactly ONE entry: `Pinky` → `${KIPRJMOD}/Pinky.pretty` |
| `PCB/sym-lib-table` | Symbol library table — references 4 `.kicad_sym` files inside `Pinky.pretty/` |
| `PCB/README.md` | JLCPCB manufacturing instructions |

## Critical library gotchas

- **`fp-lib-table` must have exactly one entry.** Two entries (or case-mismatched paths) causes the footprint editor to show nothing. The entry is: `(lib (name "Pinky") (type "KiCad") (uri "${KIPRJMOD}/Pinky.pretty") ...)`.
- **Directory name is `Pinky.pretty` (capital P).** Windows is case-insensitive, but KiCad can get confused if paths don't match. If you rename the directory, update both `fp-lib-table` and `sym-lib-table`.
- **`fp-info-cache` is auto-generated** and gets stale (orphaned footprint entries). If library issues arise, delete it and let KiCad regenerate.
- **`pinky.kicad_prl` is auto-generated** KiCad UI state (layer visibility, window positions). Not for manual editing.
- **`pinky.bak` files** occasionally appear in `Pinky.pretty/` — remove them, they don't belong in the library directory.

## Custom footprints (7)

All in `Pinky.pretty/` as `.kicad_mod`:

| Footprint | Used for |
|---|---|
| `Diode_SOD123` | 1N4148W SMD diodes (34 per board) |
| `JST_PH_reversible` | Battery connector (optional, requires jumper bridge) |
| `Jumper` | Solder jumper pads (JP1-JP4) |
| `Kailh_socket_PG1350_optional_reversible` | Kailh Choc keyswitch sockets (34 per board) |
| `MSK12C02_reversible` | Power switch |
| `nice_nano` | MCU module |
| `ResetSW` | Tactile reset button |

Plus system lib footprint: `MountingHole:MountingHole_2.2mm_M2_ISO7380` (5 per board).

## Custom symbols (5 in `Pinky.pretty/pinky.kicad_sym`)

`Device_Jumper_NC_Small`, `Device_Jumper_NO_Small`, `MJ-4PP-9`, `MX_SW_HS`, `nice_nano`.

**Symbol-to-footprint mapping to be aware of:** `Pinky:MX_SW_HS` (symbol) ↔ `Pinky:Kailh_socket_PG1350_optional_reversible` (footprint).

## Hardware & manufacturing

- **MCU:** nice!nano (nRF52840); alternatives with compatible pinout work (e.g. Puchi-BLE)
- **PCB:** 2 layers, 1.6 mm, black, HASL, no impedance control
- **Universal PCB for both halves** — same PCB built twice. Side selection handled by ZMK firmware at runtime (USB-connected half = central)
- **Solder jumper pads:**
  - JST connector mode: bridge JP2+JP3 (top) and JP1+JP4 (bottom)
  - Direct-solder mode: leave jumpers open, solder battery wires to PAD1/PAD2
- **Gerbers:** plot from KiCad to `Pinky_0-2_gerbers.zip`, upload to JLCPCB
- **Dimensions for JLCPCB preview:** 120.5 mm × 85.5 mm
- **Remove order number** — spec in `PCB/README.md`

## KiCad conventions

- **Only one footprint library** (`Pinky`). System libs (e.g. `MountingHole`) come from KiCad's global table.
- **DRC:** min track 0.127 mm, min clearance 0.0 mm, min copper-to-edge 0.5 mm
- **ERC:** all DRC violations are `error` severity for track/rules; some ERC checks are `warning` but critical ones block
- **PCB thickness: 1.6 mm** (hard requirement)
- **SMD diodes (1N4148W)** in SOD123 package on **bottom side**
- **JST PH** optional — requires jumper bridges next to jack

## Git & history

- Remote origin: `github.com:rawrex/plitka.git` (project was renamed to Pinky)
- No CI, no build scripts, no package manager — pure KiCad hardware repo
- Generated gerbers (`Pinky_0-2_gerbers.zip`) are **not tracked** — regenerate from KiCad

## What to not touch

- `PCB/.history/` and `PCB/~*.lck` — KiCad auto-save / lock files (gitignored)
- `fp-info-cache` and `pinky.kicad_prl` — auto-generated, can be deleted and regenerated
- `pinky.bak` or any `.bak` files in `Pinky.pretty/` — remove them if they appear
