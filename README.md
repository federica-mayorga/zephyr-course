# Zephyr Training Environment

Welcome to the Zephyr RTOS training! This repository includes a ready-to-use
development environment based on Zephyr 4.3.0, which you can set up in one of
three ways:

---

## Manual Zephyr Setup

Follow the following guide:
- [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#).

Make sure to select appropriate OS and to perform all steps till
[Build the Blinky Sample](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#build-the-blinky-sample).

---

# Lesson 2

## Task 1

1. Fork and set up the provided workspace.

    1. Install Zephyr SDK.
    2. Set up environment.

2. Build and run an LED blink application.

    1. Flash to your hardware board.
    2. Verify the LED toggles every second.
    3. Push tag: `l2-task1`.

3. (_Optional_) Build and run on `native_sim`.

    1. Check the boot header for the OS version.

This task demonstrates the setup of a Zephyr workspace and the successful build and execution of the Blinky sample application on both the target hardware platform and the native simulation environment.

### Workspace setup

A dedicated workspace was created and initialized using `west`. Dependencies were fetched according to the manifest configuration.

Commands used:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install west

west init -l zephyr-course
west update
west zephyr-export
```

The Zephyr SDK was automatically detected and configured for the build system.

### Checking Setup

In order to check everything was properly installed, do the following.

Open a new terminal, step into the workspace and enable the build environment:

```bash
cd zephyr-course-workspace/

source .venv/bin/activate
source ./zephyr/zephyr-env.sh
```
### Build and flash on hardware

The Blinky sample application was built and flashed to the nRF54L15 DK development board.

Command used to build:

```bash
west build -b nrf54l15dk/nrf54l05/cpuapp app/ -d build-l2-t1
```

Command used to flash:

```bash
west flash -d build-l2-t1
```

The firmware was successfully programmed into the board, and the LED toggled as expected, confirming correct execution on the physical device.

#### Evidence
![gif-from-l2-t1-native](img/l2-t1-video.gif)

### Build and execution on native simulation

The same application was built for the native simulation target and executed locally.

Command used to build:

```bash
west build -b native_sim app/ -d build-l2-t1-native
```

Command used to run:

```bash
west build -d build-l2-t1-native -t run
```

Console output confirmed the expected LED toggle behavior in the simulated environment.

#### Evidence
![screenshot-from-l2-t1-native](img/l2-t1-native.png)


### Result

This task validates the complete Zephyr development workflow, including:

* Workspace initialization and dependency management
* Toolchain and SDK configuration
* Firmware build process
* Flashing to target hardware
* Execution in native simulation

---

# Lesson 3

## Task 1

1. Reproduce a custom Kconfig menu for an LED subsystem.

    1. Create a top-level `menuconfig` option.
    2. Add a `choice` for blink sleep timing (250ms, 500ms, 1s, 2s) with hidden int symbol.
    3. Include a `menuconfig` for advanced and a `menu` for expert configuration sections.
    4. Integrate the configuration into Zephyr’s Kconfig system.

2. Build the application and verify configuration via `menuconfig`.

    1. Open the configuration interface.
    2. Navigate through the custom menu.
    3. Modify parameters and save configuration.
    4. Push tag: `l3-task1`.

This task demonstrates how to extend Zephyr’s configuration system by defining a custom Kconfig structure and integrating it into the global configuration tree.

### Kconfig Implementation

A custom `Kconfig` file was added to the application in order to define a configurable LED subsystem.

Key features implemented:

- A top-level `menuconfig` (`LED_SUBSYSTEM`)
- A `choice` for selecting blink intervals (250ms, 500ms, 1s, 2s)
- A hidden integer symbol (`LED_BLINKING_PERIOD`) derived from the selected option
- An `Advanced LED settings` submenu
- An `Expert settings` menu with conditional visibility (`visible if`)
- Use of `range` to constrain valid values

Additionally, the application Kconfig includes Zephyr’s base configuration:

```bash
source "Kconfig.zephyr"
```

This ensures that all standard Zephyr symbols (e.g., GPIO, LOG) remain available.

### Build and Configuration

The application was built using the following command:

```bash
west build -b nrf54l15dk/nrf54l15/cpuapp app/ -p -d build-l3-t1
```

To access the configuration interface:

```bash
west build -t menuconfig -d build-l3-t1
```

#### Evidence
![gif-from-l3-t1-native](img/l3-t1-video.gif)

### Result

This task validates:

* Integration of a custom Kconfig into Zephyr
* Use of menus, choices, and conditional visibility
* Parameter validation with ranges
* Interaction with Zephyr’s menuconfig system

The LED subsystem is now fully configurable at build time through a structured and user-friendly interface.

---

# Lesson 4

## Task 1

1. Create a Devicetree overlay for the application LED.

    1. Add an `app.overlay` file.
    2. Create an alias `app-led` pointing to the board `led0`.
    3. Use `DT_ALIAS(app_led)` in the application.

2. Add a configurable heartbeat period using Kconfig.

    1. Define `APP_HEARTBEAT_PERIOD_MS`.
    2. Set default value to 500.
    3. Limit valid range to 100-2000.
    4. Use the symbol as the LED blink delay.

3. Build and verify configuration via `menuconfig`.

    1. Open the configuration interface.
    2. Modify the heartbeat period.
    3. Rebuild and flash the firmware.
    4. Verify LED blink speed changes accordingly.
    5. Push tag: `l4-task1`.

This task demonstrates the integration between Zephyr Devicetree and Kconfig systems by combining hardware abstraction with configurable application behavior.

### Devicetree Overlay

An `app.overlay` file was added to define an application-specific LED alias:

```
/ {
    aliases {
        app-led = &led0;
    };
};
```

The application accesses the LED using:

```C++
#define LED_NODE DT_ALIAS(app_led)
```

### Kconfig Integration

A configurable heartbeat period was added through the application Kconfig file:

```
config APP_HEARTBEAT_PERIOD_MS
    int "Heartbeat period in milliseconds"
    range 100 2000
    default 500
```

The configuration symbol is used directly in the application:

```C++
k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
```

### Build and Configuration

The application was built using:

```bash
west build -b nrf54l15dk/nrf54l15/cpuapp app/ -p -d build-l4-t1
```

To flash the firmware:

```bash
west flash -d build-l4-t1
```

To open the configuration interface:

```bash
west build -d build-l4-t1 -t menuconfig
```

#### Evidence
![gif-from-l4-t1-native](img/l4-t1-video.gif)


### Result

This task validates:

- Usage of Devicetree overlays in Zephyr
- Creation of custom Devicetree aliases
- Use of configurable Kconfig symbols
- Integration between Devicetree, Kconfig, and application code
