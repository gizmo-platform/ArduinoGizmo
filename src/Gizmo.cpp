#include "Gizmo.h"

Gizmo::Gizmo() {}

void Gizmo::begin() {
  Wire1.setSDA(GIZMO_SYSTEM_SDA);
  Wire1.setSCL(GIZMO_SYSTEM_SCL);
  Wire1.setClock(400000);
  Wire1.begin();
}

void Gizmo::refresh() {
  // The wire format of the Gizmo processor-to-processor interconnect
  // is the classic "bag of structs" variety, which makes it easy to
  // interact with from a variety of languages.
  size_t amountRead = Wire1.requestFrom(GIZMO_ADDR, sizeof(_state));

  if (Wire1.available() >= sizeof(_state)) {
    Wire1.readBytes(reinterpret_cast<uint8_t*>(&_state), sizeof(_state));
  }
}

byte Gizmo::getAxis(int id) {
  switch(id) {
  case 0:
    return _state.Axis0;
  case 1:
    return _state.Axis1;
  case 2:
    return _state.Axis2;
  case 3:
    return _state.Axis3;
  case 4:
    return _state.Axis4;
  case 5:
    return _state.Axis5;
  default:
    return 127;
  }
}

bool Gizmo::getButton(int id) {
  switch(id) {
  case 0:
    return _state.Button0;
  case 1:
    return _state.Button1;
  case 2:
    return _state.Button2;
  case 3:
    return _state.Button3;
  case 4:
    return _state.Button4;
  case 5:
    return _state.Button5;
  case 6:
    return _state.Button6;
  case 7:
    return _state.Button7;
  case 8:
    return _state.Button8;
  case 9:
    return _state.Button9;
  case 10:
    return _state.Button10;
  case 11:
    return _state.Button11;
  default:
    return false;
  }
}
