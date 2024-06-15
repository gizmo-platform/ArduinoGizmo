#include <Servo.h>
#include "Gizmo.h"

// The Gizmo provides access to the data that is held by the field
// management system and the gizmo system processor.
Gizmo gizmo;

#include "PositionalServo.h"
#include "StatefulButton.h"
//#include "Tasks.h" //Un-comment to make use tasks for splitting up code

// Define a left and right drive motor.  Connected via MC-29 motor
// controllers they appear as servos to the system.
Servo DriveL, DriveR;

// Define a motor to sue for the arm
Servo Arm;

// Define a claw using a positional servo. This will keep the servo in a posotion as its controls are used
PositionalServo Claw;

//Create a Button to toggle into and out of Arcade Mode
StatefulButton BtnStart;

//By default, start wth Arcade mode disabled
bool bArcadeMode = false;

// Initialize the hardware and configure libraries for use.
void setup() {
  // Initialize the connection to the system processor.
  gizmo.begin();

  // Configure the I/O mode for the motor driver pins and claw
  pinMode(GIZMO_MOTOR_1, OUTPUT);
  pinMode(GIZMO_MOTOR_8, OUTPUT);
  pinMode(GIZMO_MOTOR_4, OUTPUT);
  pinMode(GIZMO_MOTOR_5, OUTPUT);

  // Configure the builtin LED so we can tell the program is running.
  pinMode(LED_BUILTIN, OUTPUT);

  // Attach the motor controller objects to the ports the motors are
  // connected to.
  DriveR.attach(GIZMO_MOTOR_1);
  DriveL.attach(GIZMO_MOTOR_8);
  Arm.attach(GIZMO_MOTOR_5);

  //Initialize the position claw on port 4 using left trigger and bumber buttons
  Claw.init(GIZMO_MOTOR_4, GIZMO_BUTTON_LT, GIZMO_BUTTON_LSHOULDER, PS_SPEED_ULTRA);


  //Use serial for Debug output
  Serial.begin(9600);

  //Give serial startup a chance to get going
  delay(500);

  Serial.println("BASIC ROBOT READY TO GO");
}

// Runs in a loop to perform tasks that are required to run the robot.
void loop() {
  // Toggle the built-in LED each time through the loop so we can see
  // that the program really is running.
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  // Refreshes the information about axis and button state.
  gizmo.refresh();

  //If start button gets clicked, toggle Arcade Mode
  if (BtnStart.clicked()) { 
    bArcadeMode = !bArcadeMode;
    Serial.printf("Drive Mode is now %s\r\n", bArcadeMode ? "ARCADE" : "TANK");
  }

  int targetL, targetR;

  if (bArcadeMode) {
    //Use arcade class to determine left and right motor speeds
    ArcadeTarget(GIZMO_AXIS_LX, GIZMO_AXIS_LY, targetL, targetR);
  } else {
    // Fetch the speed of each axis, then convert this to the range
    // expected by the motors.
    targetL = map(gizmo.getAxis(GIZMO_AXIS_LY), 0, 255, 0, 180);
    targetR = map(gizmo.getAxis(GIZMO_AXIS_RY), 0, 255, 0, 180);
  }
  
  // Write the target speeds to the motor controllers.
  DriveL.write(targetL);
  DriveR.write(targetR);

  // Update our claw position
  Claw.update();

  if (gizmo.getButton(GIZMO_BUTTON_RT)) {
    //if right trigger is pressed, send arm motor forwards
    Arm.write(180);
  } else if (gizmo.getButton(GIZMO_BUTTON_RSHOULDER)) {
    //if right shoulder is pressed, send ar motor backwards
    Arm.write(0);
  } else {
    //if neither of thr right trigger buttons is pushed, let arm know to stop
    Arm.write(90);
  }
  
  // Sleep for 50ms, which means this loop will run at ~20Hz.
  delay(50);
}
