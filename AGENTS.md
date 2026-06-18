# Pinky — AGENTS.md

## What this is

Pinky is a **34-key column-staggered choc split keyboard** — a KiCad hardware design project.  
It runs **ZMK firmware** (not in this repo). This repo contains the PCB, footprints, and build docs.

## Codebase map

| Path | What |
|---|---|
| `PCB/pinky.kicad_sch` | Schematic (KiCad v10.0, format 20260306) |
| `PCB/pinky.kicad_pcb` | PCB layout, 2-layer, 1.6 mm |
| `PCB/pinky.kicad_pro` | Project settings (DRC rules, net classes, BOM config) |
| `PCB/Pinky.pretty/` | Custom KiCad footprints (nice!nano, Kailh sockets, JST, diodes, LEDs, power switch, reset, encoder, tenting puck) |
| `PCB/fp-lib-table` | Footprint library table (only `Pinky` lib) |
| `PCB/sym-lib-table` | Symbol library table (`Pinky` + 3 rescued/jumper symbols) |
| `PCB/README.md` | Manufacturing instructions (JLCPCB, gerbers upload) |
| `docs/buildguide.md` | Full soldering build guide with photos |
| `docs/pinky_ibom.html` | Interactive HTML BOM |
| `docs/images/` | Build guide photos, pinout/trace overlays, layout SVG |

## Hardware & manufacturing

- **MCU:** nice!nano (nRF52840) — alternatives with compatible pinout work (e.g. Puchi-BLE)
- **PCB:** 2 layers, 1.6 mm, black, HASL, no impedance control  
  **Universal PCB for both halves** — the same PCB design serves both the left and right halves of the split. This is achieved through two design techniques:
  1. **Symmetric layout:** All components (MCU, switches, diodes, encoder, power switch, reset button, battery pads) are placed in mirror-symmetric positions so a single board can be built identically twice. Side selection (left vs. right central/peripheral role) is handled by the ZMK firmware at runtime — the half connected to USB becomes the central side.
  2. **Solder jumper pads:**
     - **JST connector mode:** Bridge JP2+JP3 (top side) and JP1+JP4 (bottom side) to route battery power through the JST PH jack.
     - **Direct-solder mode:** Leave all jumpers open and solder battery wires directly to the PAD1/PAD2 pads.
- **Gerbers:** generate from KiCad (plot to `Pinky_0-2_gerbers.zip`), upload to JLCPCB
- **Dimensions for JLCPCB preview:** 120.5 mm × 85.5 mm (if preview fails)
- **Remove order number** — spec in `PCB/README.md`

## KiCad conventions

- Custom footprint/symbol library is `Pinky` (no system libs pinned — the project only uses its own library + `Device` symbols)
- DRC rules: min track 0.127 mm, min clearance 0.0 mm, min copper-to-edge 0.5 mm
- ERC errors are treated as blocking (set to `error` severity)
- PCB thickness hard requirement: 1.6 mm
- SMD diodes (1N4148W) in SOD123 package on bottom side
- Encoder: EC12 flat (EC12E2440301 works best); EC11 fits but physically taller
- JST PH optional — requires bridging jumpers next to jack if used

## Git & history notes

- Remote origin: `github.com:rawrex/plitka.git` (historical name; project renamed to Pinky)
- 34-key column-staggered layout with encoders on each half
- No CI, no build scripts, no package manager — this is a pure KiCad hardware repo
- No ZMK config, keymaps, or firmware in this repo

## What to not touch

- `PCB/.history/` and `PCB/~*.lck` — KiCad auto-save / lock files (gitignored)
- Generated gerber zip — regenerate from KiCad rather than tracking in git
