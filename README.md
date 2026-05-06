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

