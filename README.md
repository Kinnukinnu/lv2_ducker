# Sidechain Ducker Plugin for Mod Audio FX Pedals

This is a work-in-progress sidechain ducker effect built with the DPF framework. My aim is to create a well-documented, easy-to-use, and reliable sidechain ducker pedal specifically for the [Mod Audio](https://mod.audio) platform. Mod Audio makes effect pedals that are basically LV2 plugin hosts running custom Linux kernel.

## Features

- **Stereo In/Out:** Full stereo signal path (Inputs 1 & 2).
- **External Sidechain:** Dedicated sidechain input (Input 3) for triggering the effect.

## Compressor Algorithm (Technical Details)

This plugin implements an aggressive **Feed-Forward External Sidechain** topology. Unlike standard digital compressors that often operate in the logarithmic (dB) domain, this ducker uses **Linear Amplitude Processing**.

* **Topology:** Feed-Forward. The gain reduction is calculated solely based on the sidechain input, leaving the main signal path pure until the final VCA stage.
* **Detector:** Asynchronous Peak Envelope Follower.
    * Uses instantaneous peak detection (`std::abs`) for immediate reaction to transients (like kick drums).
    * Attack and Release stages are decoupled 1-pole IIR filters (based on the *Citizen Chunks* example).
* **Gain Computer:** Linear Hard Knee.
    * Gain reduction is calculated in the linear domain: `Gain = 1.0 - (Overshoot * Factor)`.
    * This results in a very fast, distinct "pumping" response.

## Parameters

* **Threshold (-60dB to 0dB):** `The level at which ducking begins.`
* **Ratio (1:1 to 20:1):** `How strongly the signal is reduced once the threshold is crossed.`
* **Attack (1ms - 150ms):** `How fast the volume drops when the sidechain hits.`
* **Release (1ms - 1000ms):** `How fast the volume returns to normal.`
* **Input/Output/Sidechain Gain:** `Level trims for gain staging.`

## Development Setup

The plugin was built using a remote development cycle. I used **Visual Studio Code** on macOS to edit code via SSH, connected to a dedicated **Linux Mint** machine. This workflow combines the ergonomics of a MacBook for typing with the reliability of native building and testing of LV2 plugins on Linux.

## Current State & Roadmap

- [x] Successfully built LV2 plugin
- [x] Test on Mod Audio hardware
- [ ] Demo video
- [ ] Create GUI

---

## Building from Source

This plugin depends on the **DPF (DISTRHO Plugin Framework)**.

### 1. Prerequisites
* A C++ compiler (GCC or Clang)
* Make
* DPF (expected to be in the project root: `../../dpf`)

### 2. Compile the Plugin
Navigate to the plugin directory and build:
```bash
cd plugins/ducker
make
```

### 3. Check LV2 Metadata (Troubleshooting)
The build process *should* automatically generate the Turtle (`.ttl`) files required by LV2 hosts. 

**If the necessary `.ttl` files for LV2 format aren't generated automatically, run this:**

```bash
# 1. Ensure the generator tool is compiled (pay attention to the path!)
make -C ../../dpf/utils/lv2-ttl-generator

# 2. Move into the binary directory
cd ../../bin/ducker.lv2

# 3. Run the generator manually
# Note: Use ./ducker.so on Linux or ./ducker.dylib on macOS
../../dpf/utils/lv2-ttl-generator/lv2_ttl_generator ./ducker.so
```

### 4. Installation via Symbolic Link
Instead of manual copying, create a **Symbolic Link** to your local LV2 folder. Run this command from the **project root**:

```bash
# Create the local LV2 folder if it doesn't exist
mkdir -p ~/.lv2

# Create a symbolic link using the current path
ln -s "$(pwd)/bin/ducker.lv2" ~/.lv2/ducker.lv2
```

---

## Credits & Acknowledgements

* **Envelope Follower Algorithm:** Based on the [Citizen Chunks](https://www.musicdsp.org/en/latest/Effects/204-simple-compressor-class-c.html) example code.
* **Framework:** Built using **DPF (DISTRHO Plugin Framework)** created by falkTX.
* **DPF Tutorial & Guide:** [Audio Plugin Development with DPF](https://dev.to/reis0/audio-plugin-developmento-with-dpf-first-plugin-1jd0) by **Reis0**.

## License
MIT
