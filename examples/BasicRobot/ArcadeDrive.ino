#include "Gizmo.h"

#define MAXMAG 90

int rampStep = 0;

int SoftenMagnitude(int mag) {
 
  if (abs(mag) < 1) {
    return 0; //center deadzone  
  }

  return (mag<0 ? -1:1 )* log(abs(mag)) * MAXMAG / log(MAXMAG);
  float adj = (mag * mag) / MAXMAG;
  return (mag < 0 ? 1 : -1 )* (int)adj;
}

//Ramp up across .75 seconds
#define MAXRAMP 15

void ArcadeTarget(int XAXIS, int YAXIS, int &targetL, int &targetR) {
  int x = SoftenMagnitude(map(gizmo.getAxis(XAXIS), 0, 255, -90, 90));
	int y = SoftenMagnitude(map(gizmo.getAxis(YAXIS), 0, 255, -90, 90));

  if (abs(y)>30) x=x/2;

  if (abs(x)+abs(y)<20) {
    rampStep = 1;
  } else {
    rampStep = MIN(rampStep+1, MAXRAMP);
  }

  targetR = (MAX(MIN(y + x,90),-90)*rampStep/MAXRAMP) + MAXMAG;
  targetL = (MIN(MAX(y - x,-90),90)*rampStep/MAXRAMP) + MAXMAG;
}
