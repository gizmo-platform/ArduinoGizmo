#include <Servo.h>
#include "Gizmo.h"

// Define a left and right drive motor.  Connected via MC-29 motor
// controllers they appear as servos to the system.
Servo DriveL, DriveR;

// The Gizmo provides access to the data that is held by the field
// management system and the gizmo system processor.
Gizmo gizmo;

// Initialize the hardware and configure libraries for use.
void setup() {
  // Initialize the connection to the system processor.
  gizmo.begin();

  // Configure the I/O mode for the motor driver pins.
  pinMode(GIZMO_MOTOR_1, OUTPUT);
  pinMode(GIZMO_MOTOR_2, OUTPUT);

  // Configure the builtin LED so we can tell the program is running.
  pinMode(LED_BUILTIN, OUTPUT);

  // Attach the motor controller objects to the ports the motors are
  // connected to.
  DriveL.attach(GIZMO_MOTOR_1);
  DriveR.attach(GIZMO_MOTOR_2);
}

// Runs in a loop to perform tasks that are required to run the robot.
void loop() {
  // Toggle the built-in LED each time through the loop so we can see
  // that the program really is running.
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  // Refreshes the information about axis and button state.
  gizmo.refresh();

  // Fetch the speed of each axis, then convert this to the range
  // expected by the motors.
  int targetL, targetR;
  targetL = map(gizmo.getAxis(GIZMO_AXIS_LY), 0, 255, 0, 180);
  targetR = map(gizmo.getAxis(GIZMO_AXIS_RY), 0, 255, 0, 180);

  // Write the target speeds to the motor controllers.
  DriveL.write(targetL);
  DriveR.write(targetR);

  // Sleep for 50ms, which means this loop will run at ~20Hz.
  delay(50);
}
