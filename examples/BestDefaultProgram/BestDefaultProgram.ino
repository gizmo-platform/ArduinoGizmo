/*
This is the default BEST Robotics program for the Gizmo.
This program offers remote control of simple robots using 3 motors and a servo.
This may serve as a useful starting point for your team's competition code. You
will almost certainly need to edit or extend this code to meet your needs.

This code has two control modes: 'Tank Mode' and 'Arcade Mode'. The Start
button on your gamepad switches the robot between the two modes.

Here are the controls for Tank Mode:
Left Joystick Up/Down    - Motor 1 Fwd/Rev
Right Joystick Up/Down   - Motor 3 Fwd/Rev

Here are the controls for Arcade Mode:
Left Joystick Up/Down    - Robot Fwd/Rev
Left Joystick Left/Right - Robot Turn Left/Right

These controls work in both modes:
Right Trigger            - Motor 4 Forward
Right Shoulder Button    - Motor 4 Reverse
Left Trigger             - Servo 1 to 0 degrees
Left Shoulder Button     - Servo 1 to 90 degrees

When neither the left trigger nor shoulder button are pressed, the servo will
go to 45 degrees.
*/

#include <Gizmo.h>
#include <Servo.h>

Gizmo gizmo;

Servo motor_left;
Servo motor_right;

Servo motor_task;
Servo servo_task;

#define TANK_MODE 0
#define ARCADE_MODE 1
int mode = TANK_MODE;

bool prev_start_button = false;

void setup() {
  gizmo.begin();

  // Configure the motors & servos for the ports they are connected to
  motor_left.attach(GIZMO_MOTOR_1);
  motor_right.attach(GIZMO_MOTOR_3);
  motor_task.attach(GIZMO_MOTOR_4);
  servo_task.attach(GIZMO_SERVO_1);

  // Configure the built-in LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  /* Toggle the built-in LED each time through the loop so we can see
   * that the program really is running.
   */
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  // Refreshes the information about axis and button states
  gizmo.refresh();

  // If the start button was pressed, switch control mode
  bool start_button_pressed = gizmo.getButton(GIZMO_BUTTON_START);
  if (start_button_pressed && !prev_start_button) {
    if (mode == TANK_MODE) {
      mode = ARCADE_MODE;
    }
    else if (mode == ARCADE_MODE) {
      mode = TANK_MODE;
    }
  }
  prev_start_button = start_button_pressed;

  if (mode == TANK_MODE) {
    // Convert gamepad axis positions (0 - 255) to motor speeds (0 - 180)
    motor_left.write(map(gizmo.getAxis(GIZMO_AXIS_LY), 0, 255, 0, 180));
    motor_right.write(map(gizmo.getAxis(GIZMO_AXIS_RY), 0, 255, 0, 180));
  }
  else if (mode == ARCADE_MODE) {
    // Mix left joystick axes to control both wheels
    int speed = map(gizmo.getAxis(GIZMO_AXIS_LY), 0, 255, -90, 90);
    int steering = map(gizmo.getAxis(GIZMO_AXIS_LX), 0, 255, -90, 90);
    motor_left.write(constrain(speed - steering, -90, 90) + 90);
    motor_right.write(constrain(speed + steering, -90, 90) + 90);
  }

  // Control task motor with right trigger / shoulder button
  if (gizmo.getButton(GIZMO_BUTTON_RT)) {
    motor_task.write(0);
  }
  else if (gizmo.getButton(GIZMO_BUTTON_RSHOULDER)) {
    motor_task.write(180);
  }
  else {
    motor_task.write(90);
  }

  // Control task servo with left trigger / shoulder button
  if (gizmo.getButton(GIZMO_BUTTON_LT)) {
    servo_task.write(0);
  }
  else if (gizmo.getButton(GIZMO_BUTTON_LSHOULDER)) {
    servo_task.write(180);
  }
  else {
    servo_task.write(90);
  }
}
