#ifndef StatefulButton_h
#define StatefulButton_h

#include "Gizmo.h"

/* Stateful Button Class.  
 * This class read values for an Input from the Gizmo, and can remember state for it to return
 * transitions or current status.  Sometimes referred to as a latching button.  This class can be used on a button or a single side of an axis
 */
class StatefulButton {
public:
  StatefulButton(); //Constrctor

  /**
   * init - This function does internal setup for the Stateful Button, and must be called during setup phase
   * @param {int} BtnId - The GIZMO_BUTTON_XXX id for this button to use
   */
  void init(int BtnId);

  /**
   * init - This function does internal setup for the Stateful Button, and must be called during setup phase
   * @param {int} BtnId - The GIXMO_AXIS_XXX id for this button to use
   * @param {SBAxisPos} AxisPos - When using GIXMO_AXIS_XXX as id, Set to false for the values below 90 on the Axis and true for those above
   */
  void init(int AxisId, bool bLower);

  /**
   * clicked - See if Stateful button has just been pressed.  
   * @returns {bool} - true iff the button has just been pressed and is now in the down position
   */
  bool clicked();

  /**
   * released - See if Stateful button has just been released.  
   * @returns {bool} - true iff the button has just been released and is now in the up position
   */
  bool released();

  /**
   * isDown - See if Stateful button is actively being pushed  
   * @returns {bool} - true iff the button has is currently down
   */
  bool isDown();
  
 private:
  void update();
  int _id;
  int _lastState;
  int _state;
  int _Axis;
};

StatefulButton::StatefulButton() {}

void StatefulButton::init(int BtnId) { 
  //Init as a button
  _id = BtnId; //Set button gizmo id
  _Axis = 0;  //zero axis use
  _state = _lastState = false; //clear any state
}

void StatefulButton::init(int AxisId, bool bLower) { 
  //init as an Axis
  _id = AxisId; //Set button axis id
  _Axis = bLower ? 1 : -1; //Mark if it is for the lower or upper portion of an axis
  _state = _lastState = false; //clear any state
}

bool StatefulButton::clicked() {
  update(); //Update from gizmo
  return _state && !_lastState; //We're only clicked when gizmo said button is down, when on the last check it wasn't
}

bool StatefulButton::released() {
  update(); //Update from gizmo
  return !_state && _lastState; //We're only release when gizmo said button is up, when on the last check it wasn't
}

bool StatefulButton::isDown() {
  update(); //Update from gizmo
  return _state; //Return current state
}

void StatefulButton::update() {
  _lastState = _state; //Remember our last state <-- this is the crux of this entire control
  if (0 != _Axis) { //Using an axis control
    int val = map(gizmo.getAxis(_id), 0, 255, -10, 10); //Read in the axis.  We only worry if it is one side or the other
    _state = _Axis == (val < -1 ? -1 : (val > 1 ? 1 : 0)); //Match it to our axis, -1 for below or 1 for above
  } else {
    _state = gizmo.getButton(_id);  //using a button, just read its state
  }
}
#endif //StatefulButton_h