#pragma once

#define Block1  0
#define Block2  1
#define Block3  2
//sensor functions
extern "C" {
#include <interface.h>
}
void senseBlockCylinder2();
void senseBlockCylinder1();
bool senseBlockCylinder1value();
// given a byte value, returns the value of its bit n
int getBitValue(uInt8 value, uInt8 bit_n);

// given a byte value, set the n bit to value
void setBitValue(uInt8* variable, int n_bit, int new_value_bit);

// Led related functions

void ledRejectOff();
void ledRejectOn();

// CylinderStart related functions

/******************************************************************************************************************************
* moveCylinderStartFront - function that moves to front cylinder 0
*******************************************************************************************************************************/
void moveCylinderStartFront();

/******************************************************************************************************************************
* moveCylinderStartBack - function that moves back cylinder 0
*******************************************************************************************************************************/
void moveCylinderStartBack();

/******************************************************************************************************************************
* getCylinderStartPos - function that senses cylinder 0 (checks sensors)
*
* return 0 (if back) , 1 (if front) or -1 (if is moving)
*******************************************************************************************************************************/
int  getCylinderStartPos();

/******************************************************************************************************************************
* gotoCylinderStart- function that goes to back or end of sensors (repeats until)
*
* pos = 0 (place cylinder in back position) / pos = 1 (place cylinder in front position)
*******************************************************************************************************************************/
void gotoCylinderStart(int pos);

/******************************************************************************************************************************
* stopCylinderStart - function that stops a cylinder , stops cylinder 0
*******************************************************************************************************************************/
void stopCylinderStart();

// Cylinder1 related functions

void cylinder1FrontBack();

/******************************************************************************************************************************
* moveCylinder1Front - function that moves to front cylinder 1
*******************************************************************************************************************************/
void moveCylinder1Front();

/******************************************************************************************************************************
* moveCylinder1Back - function that moves back cylinder 1
*******************************************************************************************************************************/
void moveCylinder1Back();

/******************************************************************************************************************************
* stopCylinder1 - function that stops a cylinder , stops cylinder 1
*******************************************************************************************************************************/
void stopCylinder1();

/******************************************************************************************************************************
* getCylinder1Pos - function that senses cylinder 1 (checks sensors)
*
* return 0 (if back) , 1 (if front) or -1 (if is moving)
*******************************************************************************************************************************/
int  getCylinder1Pos();

//FAZER
/******************************************************************************************************************************
* gotoCylinder1- function that goes to back or end of sensors (repeats until)
*
* pos = 0 (place cylinder in back position) / pos = 1 (place cylinder in front position)
*******************************************************************************************************************************/
void gotoCylinder1(int pos);

// Cylinder2 related functions

void cylinder2FrontBack();

/******************************************************************************************************************************
* moveCylinder(n)Front - function that moves to front cylinder 0 , 1 or 2
*******************************************************************************************************************************/
void moveCylinder2Front();

/******************************************************************************************************************************
* moveCylinder2Back - function that moves back cylinder 2
*******************************************************************************************************************************/
void moveCylinder2Back();

/******************************************************************************************************************************
* stopCylinder2 - function that stops a cylinder , stops cylinder 2
*******************************************************************************************************************************/
void stopCylinder2();

/******************************************************************************************************************************
* getCylinder2Pos - function that senses cylinder 2 (checks sensors)
*
* return 0 (if back) , 1 (if front) or -1 (if is moving)
*******************************************************************************************************************************/
int  getCylinder2Pos();


/******************************************************************************************************************************
* gotoCylinder2- function that goes to back or end of sensors (repeats until)
*
* pos = 0 (place cylinder in back position) / pos = 1 (place cylinder in front position)
*******************************************************************************************************************************/
void gotoCylinder2(int pos);

/******************************************************************************************************************************
* ConveyorOff- function that stops the Conveyor belt
*******************************************************************************************************************************/
void ConveyorOff();


/******************************************************************************************************************************
* ConveyorOn- function that starts the Conveyor belt
*******************************************************************************************************************************/
void ConveyorOn();

/******************************************************************************************************************************
* ReadTypeValue- function that returns type of block
*******************************************************************************************************************************/
uInt8 ReadTypeValue();

void cylinderStartFrontBack();

//DEBUG
// Put here the other function headers!!!

void cylinderTest();