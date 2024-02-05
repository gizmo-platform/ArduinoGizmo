# ArduinoGizmo

This repo contains the Arduino libraries that allow you to program
your gizmo controller using the Arduino environmenrt and its Wiring
language framework.

## Prerequisites

To use this library, you must first install the Arduino enviroment and
the Raspberry Pi Pico support packages.  Instructions on installing
the support packages can be found
[here](https://arduino-pico.readthedocs.io/en/latest/install.html).

## Installation

To install this library, download the most recent release from the
releases tab.  What you will download is a ZIP archive of this
repository from a specific point in time at which the code was tested
and verified to function.  Expand this archive, which will create a
folder called "ArduinoGizmo".  This folder needs to be copied to your
Arduino environment's libraries folder, usually located at
`~/Arduino/libraries`.  On Windows, this is probably in your "My
Documents" folder.  After copying the folder into place, close and
re-open the Arduino GUI.

## Usage

To see an example of how to use this library checkout the sample code
which can be opened from `File > Examples > Gizmo > BasicRobot`. This
is a complete program that you can upload to the team processor which
is located on the right-hand side of the gizmo when held with the USB
ports facing away from you.

The example is fully annotated, but in short this library provides 4
methods as part of the Gizmo object.  All programs will need to start
off by defining a Gizmo instance outside of the `setup()` or `loop()`
functions:

```Arduino
Gizmo gizmo;
```

This creates an instance of the library that can be used later.  In
`setup()` its necessary to initialize the Gizmo components with a call
to `gizmo::begin()`.

```
setup() {
    gizmo.begin();
}
```

Then in the `loop()` function, we can use the other 3 methods provided
by the Gizmo library.  Once through the loop, ensure that your code
calls `gizmo::refresh()` to poll for new data that is received by the
system processor.  You can then access axis data with
`gizmo::getAxis(AXIS)` and button data with
`gizmo::getButton(BUTTON)`.  Data for a joystick axis is returned as
an unsigned byte with the neutral position at 127.  Data for buttons
is returned as a debounced boolean value.
