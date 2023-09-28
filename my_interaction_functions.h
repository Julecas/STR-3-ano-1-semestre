#pragma once

extern "C" {
#include <interface.h>
}

// given a byte value, returns the value of its bit n
int getBitValue(uInt8 value, uInt8 bit_n);

// given a byte value, set the n bit to value
void setBitValue(uInt8* variable, int n_bit, int new_value_bit);

// Global cylinder functions

/******************************************************************************************************************************
* stopCylinder - function that stops a cylinder , stops cylinder 0 , 1 or 2 input : 0 , 3 , 5 respetivamente
*
* bitChange = 0 -> bit 0 e 1 do cylinder 0 / bitChange = 3 -> bit 3 e 4 do cylinder 1 / bitChange = 5-> bit 5 e 6 do cylinder 2
*******************************************************************************************************************************/
void stopCylinder(int bitChange);
/******************************************************************************************************************************
* moveCylinderBack - function that moves back cylinder 0 , 1 or 2 input : 1 , 4 , 6  respetivamente
*
* bitChange = 1 -> bit 1 e 0 do cylinder 0 / bitChange = 4 -> bit 4 e 3 do cylinder 1 / bitChange = 6-> bit 6 e 5 do cylinder 2
*******************************************************************************************************************************/
void moveCylinderBack(int bitChange);
/******************************************************************************************************************************
* stopCylinder - function that moves front cylinder 0 , 1 or 2 input : 0 , 3 , 5 respetivamente
*
* bitChange = 0->bit 0 e 1 do cylinder 0 / bitChange = 3->bit 3 e 4 do cylinder 1 / bitChange = 5->bit 5 e 6 do cylinder 2
*******************************************************************************************************************************/
void moveCylinderFront(int bitChange);

// CylinderStart related functions

/******************************************************************************************************************************
* getCylinderStartPos - function that senses cylinder 0 (checks sensors)
*
* return 0 (if back) , 1 (if front) or -1 (if is moving)
*******************************************************************************************************************************/
int  getCylinderStartPos();

//FAZER
/******************************************************************************************************************************
* gotoCylinderStart- function that goes to back or end of sensors (repeats until)
*
* pos = 0 (place cylinder in back position) / pos = 1 (place cylinder in front position)
*******************************************************************************************************************************/
void gotoCylinderStart(int pos);

// Cylinder1 related functions

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

/******************************************************************************************************************************
* getCylinder2Pos - function that senses cylinder 2 (checks sensors)
*
* return 0 (if back) , 1 (if front) or -1 (if is moving)
*******************************************************************************************************************************/
int  getCylinder2Pos();

//FAZER
/******************************************************************************************************************************
* gotoCylinder2- function that goes to back or end of sensors (repeats until)
*
* pos = 0 (place cylinder in back position) / pos = 1 (place cylinder in front position)
*******************************************************************************************************************************/
void gotoCylinder2(int pos);

// Put here the other function headers!!!
