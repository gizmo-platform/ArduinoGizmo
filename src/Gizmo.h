#ifndef BESTGizmo_h
#define BESTGizmo_h

#include "Arduino.h"
#include <Wire.h>

// The Gizmo Address is the specified I2C address used by the system
// processor.  This has to be kept in sync with the gizmo-fw value in
// the unlikely event it is changed there.
const byte GIZMO_ADDR = 8;

// This structure must be kept in perfect sync with the same structure
// in the gizmo-fw definitions, since the size is what sets up the
// cast during refresh().
struct CState {
        byte Axis0;
        byte Axis1;
        byte Axis2;
        byte Axis3;
        byte Axis4;
        byte Axis5;
        byte Axis6;
        byte Axis7;

        byte Button0;
        byte Button1;
        byte Button2;
        byte Button3;
        byte Button4;
        byte Button5;
        byte Button6;
        byte Button7;
        byte Button8;
        byte Button9;
};

class Gizmo {
public:
  Gizmo();
  void begin();
  void refresh();
  byte getAxis(int);
  bool getButton(int);

 private:
  CState _state;
};

const byte GIZMO_AXIS_LX = 0;
const byte GIZMO_AXIS_LY = 1;
const byte GIZMO_AXIS_RX = 2;
const byte GIZMO_AXIS_RY = 3;
const byte GIZMO_AXIS_LT = 4;
const byte GIZMO_AXIS_RT = 5;
const byte GIZMO_AXIS_DX = 6;
const byte GIZMO_AXIS_DY = 7;

const byte GIZMO_BUTTON_BACK = 0;
const byte GIZMO_BUTTON_START = 1;
const byte GIZMO_BUTTON_LEFTSTICK = 2;
const byte GIZMO_BUTTON_RIGHTSTICK = 3;
const byte GIZMO_BUTTON_X = 4;
const byte GIZMO_BUTTON_Y = 5;
const byte GIZMO_BUTTON_A = 6;
const byte GIZMO_BUTTON_B = 7;
const byte GIZMO_BUTTON_LSHOULDER = 8;
const byte GIZMO_BUTTON_RSHOULDER = 9;

const byte GIZMO_MOTOR_1 = 14;
const byte GIZMO_MOTOR_2 = 15;
const byte GIZMO_MOTOR_3 = 16;
const byte GIZMO_MOTOR_4 = 17;
const byte GIZMO_MOTOR_5 = 18;
const byte GIZMO_MOTOR_6 = 19;
const byte GIZMO_MOTOR_7 = 20;
const byte GIZMO_MOTOR_8 = 21;

const byte GIZMO_GPIO_1 = 6;
const byte GIZMO_GPIO_2 = 7;
const byte GIZMO_GPIO_3 = 8;
const byte GIZMO_GPIO_4 = 9;
const byte GIZMO_GPIO_5 = 10;
const byte GIZMO_GPIO_6 = 11;
const byte GIZMO_GPIO_7 = 12;
const byte GIZMO_GPIO_8 = 13;

const byte GIZMO_ADC_1 = 26;
const byte GIZMO_ADC_2 = 27;
const byte GIZMO_ADC_3 = 28;

const byte GIZMO_UART_TX = 4;
const byte GIZMO_UART_RX = 5;

const byte GIZMO_SYSTEM_SDA = 2;
const byte GIZMO_SYSTEM_SCL = 3;

#endif
