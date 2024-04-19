#ifndef PositionalServo_h
#define PositionalServo_h

#include "Gizmo.h"

//Positional Speed Values for the Servo.  The higher selected, the faster the servo will go.  
//These affect the number of degrees the servo jumps per interval plus how quickly it accelerates over time while its control is depressed
typedef enum  {
  PS_SPEED_NONE = -1,
  PS_SPEED_SLOTH = 0,
  PS_SPEED_SLOW,
  PS_SPEED_MEDIUM,
  PS_SPEED_FAST,
  PS_SPEED_ULTRA
} PS_SPEED;

/* Positionaal Servo Class.  
 *  This class can be use to control a servo, maintining it at a set position or advancing it with the use of 2 buttons OR an axis.
 *  It can also add in accelearation the longer a control is held, helping give fine control or broad movements
 */
class PositionalServo {
public:
  PositionalServo(); //Constructor

  /**
   * init - This function does internal setup for the Positional Servo to be used with 2 buttons, and must be called during setup phase
   * @param {int} motorId - The GIZMO_MOTOR_XXX port where the Servo is attached
   * @param {int} btnA - used with btnB, the GIZMO_BUTTON_XXX value to decrement the servo position
   * @param {int} btnB - used with btnB, the GIZMO_BUTTON_XXX value to advance the servo position
   * @param {PS_SPEED} (OPTIONAL) - Speed servo should move, defaults to PS_SPEED_MEDIUM
   * @param {int} (OPTIONAL) defpos - servo position, defaults to 90, right in the middle
   * @param {bool} (OPTIONAL) useAcceleration - set to true iff the servo should speed up the longer a button or axis direction is held. Defaults to true
   */
  void init(int motorId, int btnA, int btnB, PS_SPEED speed = PS_SPEED_MEDIUM, int defpos = 90, bool useAcceleration = true);


  /**
   * init - This function does internal setup for the Positional Servo to be used with an axis, and must be called during setup phase
   * @param {int} motorId - The GIZMO_MOTOR_XXX port where the Servo is attached
   * @param {int} Axis - by istelf, the GIZMO_AXIS_XXX to use to decrement or advance the Servo
   * @param {PS_SPEED} (OPTIONAL) - Speed servo should move, defaults to PS_SPEED_MEDIUM
   * @param {int} (OPTIONAL) defpos - servo position, defaults to 90, right in the middle
   * @param {bool} (OPTIONAL) useAcceleration - set to true iff the servo should speed up the longer a button or axis direction is held. Defaults to true
   */
   void init(int motorId, int Axis, PS_SPEED speed = PS_SPEED_MEDIUM, int defpos = 90, bool useAcceleration = true);

  /**
   * setDefaultPosition - Set default position for the servo.  Typically defaults to 90, right in the middle.  
   * This is only needed to override a previous default set in either the init call or to setDefaultPosition
   * @param {int} defpos - set true to update the position of the servo to the enw default now
   * @param {bool} (Optional) update - set to false to prevent the position from changing to the new defualt immediately
   */
  void setDefaultPosition(int defpos, bool update=true);
  
  /**
   * setSpeed - Set speed of the servo, defaults to PS_SPEED_MEDIUM.
   * This is only needed to override a previous default set in either the init call or to setDefaultPosition
   * @param {PS_SPEED} Speed - sets the speed the servo should move, from PS_SPEED_SLOTH to PS_SPEED_ULTRA, with PS_SPEED_SLOTH being the slowest
   */
  void setSpeed(PS_SPEED Speed);
  
   /**
   * setAcceleration - Set whether the servo should accelartion th3 longer a button or axis position is held, default to true.
   * This is only needed to override a previous default set in either the init call or to setDefaultPosition
   * @param {bool} useAcceleration - set to true if the servo should speed up the longer a button or axis direction is held
   */
  void setAcceleration(bool useAcceleration);

  /**
   * reset - Reset a servo back to its defualt position
   */
  void reset();
  
  /**
   * update - Let a Servo process its position during the main loop
   * @param {bool} (OPTIONAL) bReverse) - Set true to move the servo in the opposite direction
   */
  void update(bool bReverse = false);
  
 private:
  void init(int motorId, PS_SPEED speed, int defpos, bool useAcceleration);
  int _initPos;
  int _pos;
  int _last;
  int _step;
  int _count;
  int _btnUp;
  int _btnDown;
  int _Axis;
  PS_SPEED _lastSpeed;
  int _StepInt;
  int _StepSize;
  int _StepInc;
  int _StepMax;
  
  Servo _motor;
};


PositionalServo::PositionalServo() {}

//Private Init function for inernal ffinal setup
void PositionalServo::init(int motorId, PS_SPEED speed, int defpos, bool useAcceleration) {
  _last = _step = _count = 0;   //reset all counters
  _initPos = _pos = defpos;     //Set initial and defual tpositions
  _StepInt = useAcceleration ? 1 : -1;  //Set initial step for accelerations
  setSpeed(speed);              //This call set final step interval, size and magnitude for accelration
  _motor.attach(motorId);       //Attach Motor
  reset();                      //Reset servo to default position to sync with these settings
}

void PositionalServo::init(int motorId, int btnA, int btnB, PS_SPEED speed, int defpos, bool useAcceleration) {
  //Init when using buttons
  _btnUp = btnA;          //Assign Bittons
  _btnDown = btnB;
  _Axis = -1;             //Clear Axis
  init(motorId, speed, defpos, useAcceleration); //Finish common init
}

void PositionalServo::init(int motorId, int Axis, PS_SPEED speed, int defpos, bool useAcceleration) {
  //Init when using an axis
  _btnUp = _btnDown = -1;   //CLear Buttons
  _Axis = Axis;             //Assign Axis
  init(motorId, speed, defpos, useAcceleration); //Finish common init
}

void PositionalServo::setDefaultPosition(int defPos, bool update) {
  //Set a new default posiiton.
  _initPos = defPos;
  if (update) { reset(); }
}
  

void PositionalServo::setSpeed(PS_SPEED Speed) {
  //Set servo speed. Servo speed is based on 3 factors:
  // _StepInt - interval, how many loops have to go by before an update to acceleration is consiedered
  // _StepSize - How many degrees to jump when changing positions, static when acceleration is not being used, otherwise growing
  // _StepMax - Maximum step sie to grow to, when accelerating
  _lastSpeed = Speed == PS_SPEED_NONE ? _lastSpeed : Speed; // If speed is none, this is just an update of the speed to reset Int, Size, and Max
  switch(_lastSpeed) {
    case PS_SPEED_SLOTH:  //Go real slow
      if (_StepInt>0) _StepInt = 5;
      _StepSize = 2;
      _StepMax = 30;
      break;

    case PS_SPEED_SLOW: //Go kinda slow
      if (_StepInt>0) _StepInt = 4;
      _StepSize = 3;
      _StepMax = 30;
      break;
  
    case PS_SPEED_MEDIUM: //Go middle-ish
      if (_StepInt>0) _StepInt = 3;
      _StepSize = 6;
      _StepMax = 40;
      break;
    
    case PS_SPEED_FAST: //Go Kinda Fast
      if (_StepInt>0) _StepInt = 2;
      _StepSize = 10;
      _StepMax = 50;
      break;

    case PS_SPEED_ULTRA: //Go Fast
    default:
      if (_StepInt>0) _StepInt = 1;
      _StepSize = 20;
      _StepMax = 60;
      break;
  }

}
 
void PositionalServo::setAcceleration(bool useAcceleration) {
  //Set whether to use acceleration or not.  Setting Int below zero negates acceleration
  _StepInt = useAcceleration ? 1 : -1;
  setSpeed(PS_SPEED_NONE);
}

void PositionalServo::reset() {
  //Reset to out initial positions.  Clear any counters
  _pos = _initPos;
  _last = _step = _count = 0;
  _motor.write(_initPos);
}

void PositionalServo::update(bool goReverse) {
  //Cacluate and update our position, checking if controls are active
  bool btnUp = false;
  bool btnDown = false;

  if (_btnUp != -1 && _btnDown != -1) { //Using buttons controls, get buttin states from gizmo
    btnUp = gizmo.getButton(_btnUp);
    btnDown = gizmo.getButton(_btnDown);
  } else if (_Axis != -1) { //Using axis controls, get axis state from gizmo, but just treat it as tertiary, its eitehr considered full tilt on way, the other, or in the middle
    int val = map(gizmo.getAxis(_Axis), 0, 255, -1, 1);
    btnUp = val < 0;
    btnDown = val > 0;
  }
	if (btnUp) { //Up control is is use
		if (_step <= 0) {_step = _StepSize; _count=0;} //If step has been zeroed or changed direction, reset it to beginning values and acceleration
		_pos = MIN(180,_pos+_step); //Set our new position based on step, but make sure to not go over 180
		if (_StepInt > 0 && _count++ > _StepInt) { _count = 0; _step = MIN(_step+_StepSize,_StepMax); } //if it is time to acccelerate, up the step size, but don't go over max
	} else if (btnDown) { //Down control is in use
		if (_step >= 0) {_step = -1 * _StepSize; _count=0;} //If step has been zeroed or changed direction, reset it to beginning values and acceleration
		_pos = MAX(0,_pos+_step); //Set our new position based on step, but make sure to not go below 0
		if (_StepInt > 0 && _count++ > _StepInt) { _count = 0; _step = MAX(_step-_StepSize,-1 * _StepMax); } //if it is time to acccelerate, up the step size, but don't go ov
	} else {
    //No control is use, zero out conters to reset any acceleration
		_count = 0; 
		_step = 0;
	}

	if (_last != _pos) {  //Only write if a new position has been calculated
    _motor.write(goReverse ? 180-_pos:_pos); //write it to the servo, reversing as needed
    _last = _pos; //Store this new position for future checks
  }
}

#endif //PositionalServo_h
