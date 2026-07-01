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

---

# Lesson 5

## Task 1

1. Create a custom board using the copy/rename method.

    1. Copy the contents of `deps/zephyr/boards/nordic/nrf54l15dk`.
    2. Rename the board directory and files to `our_board`.
    3. Update board identifiers, Devicetree compatibles, and Kconfig symbols.

2. Build the `hello_world` sample for the custom board.

    1. Point Zephyr to the out-of-tree board with `BOARD_ROOT`.
    2. Build for `our_board/nrf54l15/cpuapp`.
    3. Flash to the nRF54L15 DK and verify serial output.

3. Place the board directory in `<project_root>/boards/`.

4. Push tag: `l5-task1`.

This task demonstrates how to create an out-of-tree board definition by adapting an existing in-tree board and how to build a Zephyr sample against it.

### Custom Board Implementation

The nRF54L15 DK board was used as the reference and copied into `app/boards/our_board/`.

Key changes applied during the copy/rename process:

- Board directory renamed to `our_board`
- Board metadata updated in `board.yml` (`name: our_board`, `vendor: zephyr-course`)
- Variant files renamed to the `our_board_<soc>_<cluster>` pattern (`.yaml`, `.dts`, `_defconfig`)
- YAML `identifier` fields updated.
- Devicetree `compatible` strings updated.
- Shared `.dtsi` files renamed to `our_board_common.dtsi` and `our_board_nrf54l_05_10_15-pinctrl.dtsi`
- Kconfig symbols updated to `BOARD_OUR_BOARD_*` in `Kconfig.our_board`, `Kconfig`, and `Kconfig.defconfig`

Since the board lives outside the Zephyr tree, the build system must be told where to find it. The path must point to the directory that **contains** the `boards/` folder (in this case `app/`).

```bash
-DBOARD_ROOT=$PWD/app
```

### Build and Flash

Build the `hello_world` sample for the custom board (run from the `zephyr-course/` directory):

```bash
west build -b our_board/nrf54l15/cpuapp ../deps/zephyr/samples/hello_world -p -d build-l5-t1 -DBOARD_ROOT=$PWD/app
```

To flash the firmware:

```bash
west flash -d build-l5-t1
```

#### Evidence

Open a serial terminal on the board UART at 115200 baud (nRF Connect Serial Terminal, `minicom`, or `screen`). The expected output is:

![gif-from-l5-t1](img/l5-t1.png)

### Result

This task validates:

- Creation of an out-of-tree board using the copy/rename method
- Correct renaming of board files, identifiers, and Kconfig symbols
- Registration of a custom board through `BOARD_ROOT`
- Successful build of the `hello_world` sample for the custom board target

## Task 2

1. Create a custom board using the from-scratch method.

    1. Define the minimum board metadata in `board.yml`.
    2. Select the SoC in `Kconfig.scratch_board`.
    3. Describe only the required hardware in `scratch_board_nrf54l15_cpuapp.dts`.

2. Build the `hello_world` sample for said custom board.

    1. Build for `scratch_board/nrf54l15/cpuapp`.
    2. Flash to the nRF54L15 DK and verify serial output.

3. Add a board init hook that prints `Board initialized` before application `main()`.

4. Push tag: `l5-task2`.

This task demonstrates how to define a complete but minimal out-of-tree board manually, without copying an existing in-tree board definition.

### Custom Board Implementation

A second board named `scratch_board` was created in `app/boards/scratch_board/` using only the files required for the nRF54L15 application core:

```bash
app/boards/scratch_board/
├── board.yml
├── Kconfig.scratch_board
├── scratch_board_nrf54l15_cpuapp.dts
├── scratch_board_nrf54l15_cpuapp.yaml
├── scratch_board_nrf54l15_cpuapp_defconfig
├── board.cmake
├── board.c
└── CMakeLists.txt
```

`board.yml` declares the board name, vendor, and target SoC:

```yaml
board:
  name: scratch_board
  full_name: Scratch Board
  vendor: zephyr-course
  socs:
    - name: nrf54l15
```

`Kconfig.scratch_board` links the board to the correct SoC:

```kconfig
config BOARD_SCRATCH_BOARD
	select SOC_NRF54L15_CPUAPP
```

The early board message is implemented in `board.c` with a `SYS_INIT()` hook that runs before application `main()`:

```c
static int scratch_board_init(void)
{
	printk("Board initialized\n");
	return 0;
}

SYS_INIT(scratch_board_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
```

### Build and Flash

Build the `hello_world` sample for the from-scratch board (run from the `zephyr-course/` directory):

```bash
west build -b scratch_board/nrf54l15/cpuapp ../deps/zephyr/samples/hello_world -p -d build-l5-t2 -DBOARD_ROOT=$PWD/app
```

To flash the firmware:

```bash
west flash -d build-l5-t2
```

Open a serial terminal on the board UART at 115200 baud. The expected output is:

```
Board initialized
*** Booting Zephyr OS build ...
Hello World! scratch_board/nrf54l15/cpuapp
```

The custom application in `app/` can also be built against the same board to keep the LED heartbeat behavior:

```bash
west build -b scratch_board/nrf54l15/cpuapp app/ -p -d build-l5-t2-app -DBOARD_ROOT=$PWD/app
```

#### Evidence
![gif-from-l5-t2](img/l5-t2.png)

### Result

This task validates:

- Creation of a minimal out-of-tree board from scratch
- Manual definition of `board.yml`, Kconfig, and Devicetree files
- Board-level initialization before application entry point
- Successful build of the `hello_world` sample for the custom board target

---

# Lesson 6

## Task 1

1. Create a sensor driver following the driver structure from the lecture.

2. Implement `sensor_sample_fetch` and `sensor_channel_get` for an on-board LED.

    1. `sensor_sample_fetch` turns the LED on; `sensor_channel_get` turns it off.
    3. Add Devicetree binding, overlay node, Kconfig, and CMake integration.
    4. Update the application to use the sensor API instead of direct GPIO calls.

3. Build, flash, and verify LED blink and serial logs.

4. Push tag: `l6-task1`.

This task demonstrates how to implement an out-of-tree Zephyr sensor driver and integrate it with Devicetree, Kconfig, and application code.

### Driver Implementation

An out-of-tree sensor driver was added under `app/drivers/subsys/led_sensor/`.

The driver registers with the Zephyr sensor subsystem using `DEVICE_API(sensor, ...)` and `SENSOR_DEVICE_DT_INST_DEFINE`. The public API is the standard sensor interface from `<zephyr/drivers/sensor.h>`:

- `sensor_sample_fetch` drives the LED GPIO high
- `sensor_channel_get` drives the LED GPIO low and returns the last sampled value on `SENSOR_CHAN_PROX`

Directory layout:

```bash
app/
├── drivers/subsys/led_sensor/
│   ├── led_sensor.c
│   ├── CMakeLists.txt
│   └── Kconfig
├── dts/bindings/sensor/zephyr-course,led-sensor.yaml
├── dts/bindings/vendor-prefixes.txt
└── app.overlay
```

### Kconfig Integration

The driver is enabled through a `menuconfig` option sourced from the application Kconfig under **Custom drivers**:

```
menuconfig LED_SENSOR
    bool "Enable LED sensor driver"
    depends on DT_HAS_ZEPHYR_COURSE_LED_SENSOR_ENABLED
    select GPIO
    select SENSOR
```

`prj.conf` enables the driver and the sensor subsystem:

```
CONFIG_SENSOR=y
CONFIG_LED_SENSOR=y
```

CMake only compiles the driver when `CONFIG_LED_SENSOR` is set, and `BOARD_ROOT` must point to the directory that contains `boards/` (that is, `app/`).

### Devicetree Overlay

The overlay defines the sensor node and a `led-sensor` alias (same GPIO as board `led0`):

```
/ {
    aliases {
        led-sensor = &led_sensor;
    };

    led_sensor: led-sensor {
        compatible = "zephyr-course,led-sensor";
        gpios = <&gpio2 9 GPIO_ACTIVE_HIGH>;
        status = "okay";
    };
};

&led0 {
    status = "disabled";
};
```

The application obtains the device with:

```C++
#define LED_SENSOR_NODE DT_ALIAS(led_sensor)
static const struct device *const led_sensor = DEVICE_DT_GET(LED_SENSOR_NODE);
```

GPIO setup and toggle calls were replaced by `device_is_ready`, `sensor_sample_fetch_chan`, and `sensor_channel_get`, keeping the original loop structure and log format:

```C++
while (1) {
    sensor_sample_fetch_chan(led_sensor, SENSOR_CHAN_ALL);
    k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS / 2);

    sensor_channel_get(led_sensor, SENSOR_CHAN_PROX, &val);
    led_state = !led_state;
    LOG_INF("LED state: %s", led_state ? "ON" : "OFF");

    k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS / 2);
}
```

Each cycle splits `APP_HEARTBEAT_PERIOD_MS` in half: the LED is on between fetch and get, then off until the next fetch (similar duty cycle to the original GPIO toggle).

The board `led0` node is disabled in a separate overlay fragment (`&led0 { status = "disabled"; };`) because it shares the same GPIO with `led-sensor`; otherwise the `gpio-leds` driver would conflict with our sensor driver.

### Build and Flash

Build the application for the custom board (run from the `zephyr-course/` directory):

```bash
west build -b our_board/nrf54l15/cpuapp app/ -p -d build-l6-t1 -DBOARD_ROOT=$PWD/app
```

To flash the firmware:

```bash
west flash -d build-l6-t1
```

#### Evidence

The on-board LED0 should blink (on after fetch, off after get; full blink period = `APP_HEARTBEAT_PERIOD_MS`, default 500 ms).

![gif-from-l6-t1](img/l6-t1.gif)

Open a serial terminal on the board UART at 115200 baud (`minicom`).

```bash
minicom -D /dev/ttyACM1
```

> NOTE: The device /dev/ttyACM1 might has a different name on your computer.

The expected output alternates:

```
[00:00:00.250,000] <inf> main: LED state: ON
[00:00:00.500,000] <inf> main: LED state: OFF
```

![screenshot-from-l6-t1](img/l6-t1.png)

The on-board LED should blink at half the heartbeat period on and half off (full period = `APP_HEARTBEAT_PERIOD_MS`). Serial logs alternate `LED state: ON` / `OFF` each cycle.

To open the configuration interface:

```bash
west build -d build-l6-t1 -t menuconfig
```

Navigate to **Custom drivers** and confirm `CONFIG_LED_SENSOR` is enabled.

![screenshot-from-l6-t1](img/l6-t1-menuconfig.png)

### Result

This task validates:

- Implementation of an out-of-tree sensor driver with `sample_fetch` and `channel_get`
- Devicetree binding and overlay integration for a custom compatible
- Kconfig and CMake wiring to enable and build the driver
- Application use of the Zephyr sensor API instead of direct GPIO access
- Visible LED blink driven by fetch/get timing without GPIO pin conflicts

## Task 2

1. Add a custom extension API to the LED sensor driver.

    1. Define a `__subsystem` driver API with at least one function that changes a field in the dynamic data struct (`led_sensor_data`).
    2. Expose wrapper functions for the application (`led_sensor_set_sample`, `led_sensor_get_sample`).

2. Call the extension API from `main` before the sensor fetch/get cycle.

3. Build, flash, and verify serial logs show the updated `sample` value.

4. Push tag: `l6-task2`.

This task extends the Task 1 sensor driver with a vendor-specific API layer, similar to the lecture `our_driver` pattern, while keeping the standard Zephyr sensor API for fetch and get.

### Extension API Implementation

A public header was added at `app/drivers/subsys/led_sensor/include/led_sensor.h`:

```C
__subsystem struct led_sensor_driver_api {
    int (*set_sample)(const struct device *dev, int sample);
    int (*get_sample)(const struct device *dev);
};
```

The extension API table is stored in `led_sensor_config.api` and operates on `led_sensor_data.sample` (runtime data). `sensor_channel_get` returns this value in `val->val1`; `sensor_sample_fetch` only controls the GPIO and no longer overwrites `sample`, so values set through the extension API are preserved.

Wrapper functions in `led_sensor.c` dispatch through the config pointer:

```C
int led_sensor_set_sample(const struct device *dev, int sample);
int led_sensor_get_sample(const struct device *dev);
```

The driver CMakeLists adds the include path for the application:

```cmake
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
```

### Application Changes

`main` includes `<led_sensor.h>` and calls the extension API each loop iteration before `sensor_sample_fetch_chan`:

```C
led_sensor_set_sample(led_sensor, led_state ? 1 : 0);
sensor_sample_fetch_chan(led_sensor, SENSOR_CHAN_ALL);
/* ... */
LOG_INF("LED state: %s, sample: %d", led_state ? "ON" : "OFF",
        led_sensor_get_sample(led_sensor));
```

### Build and Flash

Build and flash from the `zephyr-course/` directory (same board and `BOARD_ROOT` as Task 1):

```bash
west build -b our_board/nrf54l15/cpuapp app/ -p -d build-l6-t2 -DBOARD_ROOT=$PWD/app
```

To flash the firmware:

```bash
west flash -d build-l6-t2
```

Open a serial terminal on the board UART at 115200 baud (`minicom`).

```bash
minicom -D /dev/ttyACM1
```

> NOTE: The device /dev/ttyACM1 might has a different name on your computer.

#### Evidence

The serial log must include the `sample` field, alternating between `0` and `1`, which confirms the extension API is updating `led_sensor_data.sample` and `led_sensor_get_sample()` is reachable from the application:

```
[00:00:00.500,000] <inf> main: LED state: OFF, sample: 1
[00:00:01.000,000] <inf> main: LED state: ON, sample: 0
[00:00:01.500,000] <inf> main: LED state: OFF, sample: 1
```

![screenshot-from-l6-t2](img/l6-t2.png)

> The LED continues to blink as in Task 1; the screenshot above is the primary evidence for Task 2.

### Result

This task validates:

- Definition of a custom `__subsystem` extension API alongside the Zephyr sensor driver API
- Modification of a runtime (`dev->data`) field through the extension API
- Application use of driver-specific wrappers instead of accessing driver data directly
- Correct interaction between extension API, `sample_fetch` / `channel_get`, and logging

---

# Lesson 7

## Task 1

1. Expose the LED sensor through the Zephyr shell.

    1. Add a root `sensor` command with subcommands `fetch`, `read`, and `info`.
    2. Wire each subcommand to the corresponding Zephyr sensor API.
    3. Enable serial shell support in the application configuration.
    4. Build, flash, and validate the commands over the UART console.

2. Push tag: `l7-task1`.

This task demonstrates how to expose a custom sensor driver through the Zephyr shell so it can be inspected and exercised at runtime without changing the firmware for every test.

### Shell Integration

The application registers a shell command tree in `app/src/main.cpp` for the existing LED sensor driver:

- `sensor fetch` calls `sensor_sample_fetch()`
- `sensor read` calls `sensor_channel_get()` for `SENSOR_CHAN_PROX`
- `sensor info` prints the device name and readiness state

The handlers are registered using the Zephyr shell macros `SHELL_STATIC_SUBCMD_SET_CREATE` and `SHELL_CMD_REGISTER`.

Serial shell support is enabled in `app/prj.conf`:

```text
CONFIG_SHELL=y
CONFIG_SHELL_BACKEND_SERIAL=y
```

### Build and Flash

Build and flash the application from the `zephyr-course/` directory:

```bash
west build -b our_board/nrf54l15/cpuapp app/ -p -d build-l7-t1 -DBOARD_ROOT=$PWD/app
```

```bash
west flash -d build-l7-t1
```

Open a serial terminal at 115200 baud (`minicom` or similar):

```bash
minicom -D /dev/ttyACM1
```

> NOTE: The device `/dev/ttyACM1` may have a different name on your computer.

Once the board is running, the following commands can be used from the shell:

```text
uart:~$ sensor fetch
sensor_sample_fetch() called

uart:~$ sensor read
prox: 0.000000

uart:~$ sensor info
name: led-sensor
ready: yes
```

The LED heartbeat loop continues to run in the background while the shell commands are available for interactive inspection.

#### Evidence

![screenshot-from-l7-t1](img/l7-t1.png)

### Result

This task validates:

- Integration of a custom Zephyr driver with the shell subsystem
- Runtime inspection of sensor behavior through serial commands
- Use of standard sensor APIs from a shell command path
- Easier debugging and testing of driver functionality on the target board

---

## Task 2

1. Expose the extension API from Lesson 6 Task 2 as a shell subcommand.

    1. Add a `sensor ser <value>` command.
    2. Use `SHELL_CMD_ARG` so the shell enforces the argument count.
    3. Validate the input value and print an error with `shell_error` when it is missing or out of range.
    4. Build, flash, and validate the command over the UART console.

2. Push tag: `l7-task2`.

This task demonstrates how to expose a custom driver extension API through the Zephyr shell so its runtime behavior can be changed interactively.

### Shell Integration

The application adds a new shell handler in `app/src/main.cpp` for the custom extension API from Lesson 6 Task 2:

- `sensor ser <value>` updates the internal sample value used by the sensor driver
- The command accepts only `0` or `1`
- Missing or invalid values are rejected with `shell_error`

The argument count is enforced by `SHELL_CMD_ARG`, and the handler validates the value before calling `led_sensor_set_sample()`.

### Build and Flash

Build and flash the application from the `zephyr-course/` directory:

```bash
west build -b our_board/nrf54l15/cpuapp app/ -p -d build-l7-t2 -DBOARD_ROOT=$PWD/app
```

```bash
west flash -d build-l7-t2
```

Open a serial terminal at 115200 baud (`minicom` or similar):

```bash
minicom -D /dev/ttyACM1
```

> NOTE: The device `/dev/ttyACM1` may have a different name on your computer.

Once the board is running, the following commands can be used from the shell:

```text
uart:~$ sensor ser 1
sample set to 1

uart:~$ sensor read
prox: 1.000000

uart:~$ sensor ser 0
sample set to 0

uart:~$ sensor read
prox: 0.000000
```

The command also rejects invalid input:

```text
uart:~$ sensor ser 2
value must be 0 or 1
```

#### Evidence

![screenshot-from-l7-t2](img/l7-t2.png)

### Result

This task validates:

- Exposure of a custom driver extension API through the shell
- Validation of shell arguments with `SHELL_CMD_ARG`
- Runtime update of sensor state through a serial command
- Easier interactive testing of driver behavior on the target board

---

# Lesson 8

## Task 1

1. Implement the missing unit tests for the `ring_buf` module.

    1. Fill the 7 stub `ZTEST` bodies in `tests/ring_buf/src/test_ring_buf.c`.
    2. Verify all 8 tests pass with Twister.

2. Push tag: `l8-task1`.

This task demonstrates how to write black-box unit tests for a small Zephyr module using ZTest. The tests exercise the public API of the `ring_buf` module without depending on its internal implementation details.

### Test Implementation

The test suite covers the following behaviors:

- Reinitialization clears previously stored state
- A single push/pop round-trip works correctly
- FIFO order is preserved across multiple pushes and pops
- Pushing when the buffer is full returns `-ENOSPC`
- `rb_peek()` does not consume the element
- `rb_pop(NULL)` returns `-EINVAL`
- `rb_is_full()` becomes true after the buffer is filled

The implementation is located in `tests/ring_buf/src/test_ring_buf.c` and uses `zassert_*` assertions to validate each expected behavior.

### Verification

The tests were run with:

```bash
west twister -T tests/ring_buf -p native_sim -v
```

Observed result:

```text
INFO    - 1 test scenarios (1 configurations) selected
INFO    - 8 of 8 executed test cases passed (100.00%)
```

#### Evidence

![screenshot-from-l8-t1](img/l8-t1.png)

### Result

This task validates:

- Writing unit tests for a Zephyr module with ZTest
- Covering initialization, push/pop, FIFO order, and boundary behavior
- Verifying the public API through black-box tests
- Running and confirming the test suite with Twister

## Task 2

1. Run the coverage report focused on the `ring_buf` module.

2. Open `twister-out/coverage/index.html` and verify the expected coverage numbers.

3. Include the `twister-out/coverage/` directory in the commit.

4. Push tag: `l8-task2`.

This task extends the unit testing work by generating a coverage report for the `ring_buf` implementation and confirming that the module reaches the expected thresholds.

### Coverage Report

The coverage report was generated with:

```bash
west twister -T tests/ring_buf -p native_sim \
    --coverage --coverage-tool gcovr \
    --coverage-basedir app/modules/ring_buf
```

The generated report was opened in `twister-out/coverage/index.html` and verified against the expected values:

```text
Lines: 81.4%
Functions: 100.0%
Branches: 64.3%
```

#### Evidence

![screenshot-from-l8-t2](img/l8-t2.png)

### Result

This task validates:

- Generating a Twister coverage report for a Zephyr module
- Verifying module coverage thresholds for lines, functions, and branches
- Including the coverage output in the repository history
- Tagging the completed work as `l8-task2`
