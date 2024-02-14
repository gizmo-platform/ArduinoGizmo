#include "Gizmo.h"

// The Gizmo provides access to the data that is held by the field
// management system and the gizmo system processor.
Gizmo gizmo;

// Initialize the hardware and configure libraries for use.
void setup() {
  // Initialize the connection to the system processor.
  gizmo.begin();

  pinMode(LED_BUILTIN, OUTPUT);
}

// Loop runs continuously.
void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  gizmo.refresh();

  Serial.printf("Axis Data: %i %i %i %i %i %i; ",
                gizmo.getAxis(GIZMO_AXIS_LX),
                gizmo.getAxis(GIZMO_AXIS_LY),
                gizmo.getAxis(GIZMO_AXIS_RX),
                gizmo.getAxis(GIZMO_AXIS_RY),
                gizmo.getAxis(GIZMO_AXIS_DX),
                gizmo.getAxis(GIZMO_AXIS_DY)
                );
Serial.printf("Button Data: %i %i %i %i %i %i %i %i %i %i %i %i",
              gizmo.getButton(GIZMO_BUTTON_BACK),
              gizmo.getButton(GIZMO_BUTTON_START),
              gizmo.getButton(GIZMO_BUTTON_LEFTSTICK),
              gizmo.getButton(GIZMO_BUTTON_RIGHTSTICK),
              gizmo.getButton(GIZMO_BUTTON_X),
              gizmo.getButton(GIZMO_BUTTON_Y),
              gizmo.getButton(GIZMO_BUTTON_A),
              gizmo.getButton(GIZMO_BUTTON_B),
              gizmo.getButton(GIZMO_BUTTON_LSHOULDER),
              gizmo.getButton(GIZMO_BUTTON_RSHOULDER),
              gizmo.getButton(GIZMO_BUTTON_LT),
              gizmo.getButton(GIZMO_BUTTON_RT)
              );
  Serial.println();
  delay(100);
}
