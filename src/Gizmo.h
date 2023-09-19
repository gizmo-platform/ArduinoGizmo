#ifndef BESTGizmo_h
#define BESTGizmo_h

#include "Arduino.h"
#include "<Wire.h>"

// The Gizmo Address is the specified I2C address used by the system
// processor.  This has to be kept in sync with the gizmo-fw value in
// the unlikely event it is changed there.
#define GIZMO_ADDR 8

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
  Gizmo(int, int);
  void begin();
  void refresh();
  byte getAxis(int);
  bool getButton(int);

 private:
  int _sda;
  int _sdc;
  CState _state;
}
#endif
