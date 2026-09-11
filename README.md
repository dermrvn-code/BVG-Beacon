<div align="center">
  <h1>BVG Beacon</h1>
  <p>
  <strong>A tiny real-time departure display for Berlin's public transport.</strong>
  <br>
  Built around an ESP32-C3 and a 1.28" round TFT display.
  </p>
</div>

<p align="center">
  <a href="https://github.com/dermrvn-code/BVG-Beacon">
    <img src="https://img.shields.io/github/stars/dermrvn-code/BVG-Beacon?style=for-the-badge&color=F5C542" alt="GitHub stars">
  </a>
  <a href="https://github.com/dermrvn-code/BVG-Beacon/issues">
    <img src="https://img.shields.io/github/issues/dermrvn-code/BVG-Beacon?style=for-the-badge&color=E05D44" alt="GitHub issues">
  </a>
  <a href="https://github.com/dermrvn-code/BVG-Beacon">
    <img src="https://img.shields.io/github/last-commit/dermrvn-code/BVG-Beacon?style=for-the-badge&color=2EA44F" alt="Last commit">
  </a>
  <br>
  <img src="https://img.shields.io/badge/status-prototype-orange?style=for-the-badge" alt="Project status: prototype">
</p>

---

## What is BVG Beacon?

**BVG Beacon** is a small, standalone departure display for Berlin's public transport network.

It uses an **ESP32-C3** and a **1.28" round TFT display** to retrieve live departure information and show it at a glance.

The Beacon connects to your Wi-Fi and retrieves departure data through the [BVG transport REST API](https://v6.bvg.transport.rest/). Stations and lines can be configured through a lightweight web interface hosted directly by the ESP32.

The entire project is designed to be reproducible:

* Hardware documentation
* Firmware
* Web interface
* Configuration files
* 3D-printable enclosure
* Build instructions

are all contained in this repository.

> **Project status:** This is currently a prototype. Things may change, break, or be reorganized as the project develops.

---

## Features

* Real-time public transport departures
* Compact **1.28" round TFT** display
* ESP32-C3 based
* Wi-Fi connectivity
* Wi-Fi configuration through an access point on first boot
* Browser-based configuration interface
* Configure multiple stations
* Configure multiple lines
* No separate server required
* 3D-printable enclosure
* Open-source firmware and hardware files

---

## Documentation

* [Hardware](#hardware)
* [Enclosure](#enclosure)
* [Software](#software)
* [First Boot](#first-boot)
* [Configuration](#configuration)
* [Buttons](#buttons)
* [Data Source](#data-source)
* [Project Structure](#project-structure)
* [Troubleshooting](#troubleshooting)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)

---

# Hardware

## Bill of Materials

| Part                     | Quantity | Notes                         |
| ------------------------ | -------: | ----------------------------- |
| ESP32-C3 Mini            |        1 | Main microcontroller          |
| GC9A01 1.28" TFT display |        1 | Round SPI display             |
| Tactile push button      |        2 | User input                    |
| Resistors                |      TBD | See wiring diagram            |
| Wires                    |        — | For connecting the components |
| USB-C cable              |        1 | Power and programming         |
| USB power supply         |        1 | 5 V USB power source          |
| 3D-printed enclosure     |        1 | Included in the repository    |

> **TODO:** Add exact resistor values, wire lengths, connector types and any additional components.

---

## Wiring

> **TODO:** Add a wiring diagram.

<!-- Add the wiring diagram here once it is available. -->

### ESP32-C3 → GC9A01

The current TFT configuration uses:

| GC9A01     | ESP32-C3 |
| ---------- | -------: |
| MOSI / SDA |   GPIO 6 |
| SCLK / SCL |   GPIO 4 |
| CS         |   GPIO 7 |
| DC         |   GPIO 8 |
| RST        |  GPIO 10 |

> **Note:** Pin assignments may change during development. Check the configuration in the repository before wiring a new device.

> **TODO:** Add button pin assignments.

---

# Enclosure

The enclosure is designed to be 3D printed and is included with the project as 3MF files.

> **TODO:** Add photographs of the enclosure and assembly.

## Files

The `3D-Models/` directory contains the printable enclosure files.

> **TODO:** Add exact file names once the enclosure is finalized.

## Suggested print settings

> **TODO:** Add recommended:
>
> * Layer height
> * Infill
> * Filament
> * Supports
> * Nozzle size
> * Print orientation
> * Approximate print time

---

# Software

## Requirements

To build the firmware, you will need:

* [PlatformIO](https://platformio.org/)
* A USB connection to the ESP32-C3
* A Wi-Fi network for the finished device

The project uses PlatformIO to manage the firmware build and dependencies.

---

## Installation

### 1. Clone the repository

```bash
git clone https://github.com/dermrvn-code/BVG-Beacon.git
cd BVG-Beacon
```

### 2. Open the project in PlatformIO

Open the repository in your preferred PlatformIO-compatible editor.

---

## Build and Flash

### 1. Build the firmware

Use PlatformIO to build the project.

### 2. Build the filesystem image

Run:

```text
PlatformIO → Build Filesystem Image
```

### 3. Upload the filesystem

Run:

```text
PlatformIO → Upload Filesystem Image
```

### 4. Upload the firmware

Run:

```text
PlatformIO → Upload
```

The ESP32-C3 should now boot into the BVG Beacon firmware.

---

# First Boot

On the first boot, the Beacon starts a temporary Wi-Fi access point.

The display will guide you through the setup process.

The general setup flow is:

```text
┌──────────────────────┐
│      Power On        │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│    Beacon starts     │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│   Wi-Fi Access Point │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│ Configure Wi-Fi      │
│ through your browser │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│ Connect to your      │
│ normal Wi-Fi network │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│  Start displaying    │
│     departures       │
└──────────────────────┘
```

Once configured, the Beacon will connect to the configured Wi-Fi network automatically.

---

# Configuration

The Beacon contains a small web interface that can be accessed from any device on the same network.

To open it:

1. Make sure your computer or phone is connected to the same Wi-Fi network as the Beacon.
2. Press **both buttons simultaneously** on the Beacon.
3. Open the displayed IP address in a web browser.
4. Configure your stations and lines.

<!-- Add a screenshot of the configuration interface here once it is available. -->

## What can be configured?

The configuration interface allows you to customize which departures are displayed.

> **TODO:** Confirm the exact available configuration options and document them here.

Currently planned/documented options include:

* Stations
* Lines
* Departure configuration

---

# Buttons

The Beacon uses two tactile buttons for basic interaction.

> **TODO:** Document the final button mapping.

| Action       | Function                             |
| ------------ | ------------------------------------ |
| Button 1     | TBD                                  |
| Button 2     | TBD                                  |
| Both buttons | Open configuration / show IP address |

---

# Data Source

BVG Beacon retrieves public transport departure information through the [BVG transport REST API](https://v6.bvg.transport.rest/).

The API provides access to Berlin public transport information, including stops, departures and other transport data.

The Beacon does **not** require its own backend server; the ESP32 communicates with the API directly.

> **Important:** The availability, behavior, rate limits and terms of external APIs can change independently of this project. If the API changes or becomes unavailable, the Beacon may stop receiving departure data.

---

# Project Structure

The repository is roughly organized as follows:

```text
BVG-Beacon/
├── src/                    # Firmware source
├── include/                # Header files
├── data/                   # Web interface / filesystem
├── lib/                    # Project-specific libraries
├── 3D-Models/              # 3D-printable enclosure
├── docs/
│   └── images/             # Documentation images
├── platformio.ini          # PlatformIO configuration
└── README.md
```

> **TODO:** Update this tree to match the actual repository structure.

---

# Troubleshooting

## The display stays blank

Check:

* TFT wiring
* SPI pin configuration
* `Setup200_GC9A01.h`
* TFT power
* Display reset wiring

Make sure the configured GPIOs match the actual wiring.

---

## The Beacon does not connect to Wi-Fi

On first boot, connect to the Beacon's temporary access point and configure your Wi-Fi credentials again.

If necessary, restart the ESP32-C3 and repeat the setup process.

---

## No departures are shown

Check:

1. The Beacon has a working Wi-Fi connection.
2. The configured station exists.
3. The configured line exists.
4. The BVG transport API is reachable.
5. The API has not changed its response format.

You can test the API independently by opening:

https://v6.bvg.transport.rest/

---

# Roadmap

The project is currently a prototype, so expect this list to change.

* [ ] Finish and document the enclosure
* [ ] Add complete wiring diagram
* [ ] Add assembly instructions
* [ ] Add photos of the finished Beacon
* [ ] Add web UI screenshots
* [ ] Improve first-time setup
* [ ] Move TFT_eSPI configuration into the project (currently not possible)
* [ ] Improve error handling
* [ ] Add better display states for network/API errors
* [ ] Document button interactions
* [ ] Finalize hardware documentation
* [ ] Produce a first stable release

---

# Contributing

Contributions, improvements and ideas are welcome.

If you find a bug or have an idea for a feature, feel free to open an issue.

For larger changes, opening an issue first is recommended so the proposed change can be discussed before significant work is done.

---

# Disclaimer

**BVG Beacon is an independent, unofficial project.**

It is not affiliated with, endorsed by or sponsored by **Berliner Verkehrsbetriebe (BVG)**.

The name **BVG**, the BVG logo, visual identity, trademarks, imagery and other BVG intellectual property are not part of the project's open-source license.

This project uses public transport data provided through the BVG transport REST API. The API and its underlying data are subject to their respective terms and conditions.

> **TODO:** Verify the current API terms and add the appropriate attribution here before publishing a release.

For official BVG information, visit the [BVG website](https://www.bvg.de/).

---

# License

## Software

The software contained in this repository is licensed under the [**MIT License**](LICENSE).

## Hardware and 3D Models

The hardware designs and 3D-printable files are licensed under the [**CERN-OHL-S-2.0**](LICENSE-HARDWARE).

### Third-party materials

Third-party libraries, fonts, graphics and other dependencies remain under their respective licenses.

BVG trademarks, logos, visual identity and other BVG-owned materials are **not** licensed under the project's MIT or CERN-OHL-S licenses.

---

# Credits

Built in Berlin, for Berlin.

BVG Beacon uses the following open-source libraries and services:

* [PlatformIO](https://platformio.org/) — development environment, build system and dependency management
* [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) by Bodmer — display driver and graphics library
* [PNGdec](https://github.com/bitbank2/PNGdec) by Larry Bank — PNG image decoding
* [WiFiManager](https://github.com/tzapu/WiFiManager) by tzapu — Wi-Fi configuration portal and access-point setup
* [BVG transport REST API](https://v6.bvg.transport.rest/) — public transport data

The project also uses the [Espressif RISC-V toolchain package](https://registry.platformio.org/tools/platformio/toolchain-riscv32-esp) provided through PlatformIO.

---

<p align="center">
  <sub>Made for people who like knowing when the next train is coming.</sub>
</p>
