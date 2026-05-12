# BYU Telemetry HUD

A real-time C++ telemetry heads-up display for BYU Rocketry flight data. The app
uses raylib to render a fullscreen mission-control style HUD, consumes CSV
telemetry from a serial device, estimates the rocket state, and visualizes live
altitude, velocity, orientation, sensor values, and link health.

## Main Features

- Live serial telemetry ingestion from a configured USB serial port.
- CSV telemetry parser for IMU, magnetometer, barometer, altitude, and high-G
  accelerometer samples.
- Startup calibration flow using a rolling window of initial samples to collect
  accelerometer and gyroscope biases.
- Rocket state updates for altitude, vertical velocity, per-axis velocity, sample
  rate, and quaternion-based orientation.
- Fullscreen raylib HUD with:
  - 3D rocket model rendered with a GLSL lighting shader.
  - Altitude ladder with mission milestones and a current-altitude marker.
  - Flight-stage indicator panel.
  - Sensor readouts for velocity, attitude, and altitude.
  - Link-status panel for samples per second and packet-drop tracking.
  - Altitude graph panel.
  - Camera-feed placeholder panel.
- Bundled sample telemetry and flight-analysis data under `data/`.
- Bundled visual resources under `resources/`, including the rocket model,
  shader files, and HUD font.

## Repository Layout

```text
src/
  main.cpp                    App entry point and serial-port configuration
  hud/                        raylib app setup, layout, colors, and drawing
  hud/draw/                   HUD panels, graph, telemetry, and sensor widgets
  state/                      Rocket state, calibration, and update logic
  telemetry/                  Serial input, CSV parsing, sample buffers, stats
tests/
  test.cpp                    Minimal HUD smoke target
resources/
  fonts/                      HUD font assets
  models/                     Rocket GLB model
  shaders/                    GLSL lighting shaders
data/                         Sample telemetry and FAR test-flight data
```

## Requirements

- CMake 3.20 or newer.
- A C++20-capable compiler.
- raylib installed with CMake package support.
- A serial telemetry device that emits the expected CSV format.

On macOS, raylib can commonly be installed with Homebrew:

```sh
brew install cmake raylib
```

Other install methods are fine as long as CMake can satisfy:

```cmake
find_package(raylib CONFIG REQUIRED)
```

## Build

```sh
cmake -S . -B build
cmake --build build
```

This builds two executables:

- `build/BYU_Telemetry_HUD` - the main telemetry HUD.
- `build/test` - a simple HUD smoke target.

## Run

Before launching, connect the telemetry device at the configured serial path.
The current app entry point uses:

```cpp
SerialPort serial = SerialPort("/dev/cu.usbserial-0001", 57600);
```

To use a different device or baud rate, edit `src/main.cpp`.

Then run:

```sh
./build/BYU_Telemetry_HUD
```

The app opens fullscreen. Click `CALIBRATE`, keep the rocket/sensor package
still while the initial sample window fills, and then the HUD switches into the
live display.

## Telemetry Format

Incoming serial data is expected to be newline-delimited CSV with exactly 17
fields per row:

```csv
t_us,ax,ay,az,gx,gy,gz,mx,my,mz,imuTempC,baroTempC,pressPa,altM,hgx,hgy,hgz
```

Field groups:

- `t_us`: sample timestamp in microseconds.
- `ax, ay, az`: normal accelerometer readings.
- `gx, gy, gz`: gyroscope readings.
- `mx, my, mz`: magnetometer readings.
- `imuTempC`: IMU temperature in Celsius.
- `baroTempC, pressPa, altM`: barometer temperature, pressure, and altitude.
- `hgx, hgy, hgz`: high-G accelerometer readings.

Sample logs in this format are included in `data/log_0001.csv`,
`data/log_0007.csv`, and `data/log_0037.csv`.

## Configuration Points

Most display constants live in `src/hud/config.hpp`, including target FPS, model
path, scene sizing, graph scale, altitude-bar scale, text sizes, and unit
conversions.

HUD colors are defined in `src/hud/colors.hpp`. The active palette is selected
by `COLOR_PALETTE` in `src/hud/config.hpp`.

Altitude milestones shown on the altitude ladder are defined in
`src/hud/milestones.hpp`.

## Current Implementation Notes

- The flight-stage UI supports Pad, Boost, Coast, Apogee, Descent, and Recovery.
  The current state update path advances from calibration to Pad; automatic
  flight-stage detection is not yet implemented.
- The camera feed panel is currently a placeholder.
- Packet-drop and altitude-graph display support exists in the HUD, but the
  current main loop does not yet populate all of those values.
- The app expects the serial device to be available at startup.

## Verified

The project currently builds successfully with:

```sh
cmake --build build
```
