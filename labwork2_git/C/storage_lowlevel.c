#include <stdio.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>

#include <storage.h>


void initializeHardwarePorts() {
    createDigitalInput(0);
    createDigitalInput(1);
    createDigitalOutput(2);

    writeDigitalU8(2, 0x00);
}


void setBitValue(uInt8* variable, int n_bit, int new_value_bit)
// given a byte value, set the n bit to value
{
    uInt8  mask_on = (uInt8)(1 << n_bit);
    uInt8  mask_off = ~mask_on;
    if (new_value_bit)  *variable |= mask_on;
    else                *variable &= mask_off;
}

int getBitValue(uInt8 value, uInt8 n_bit)
// given a byte value, returns the value of bit n
{
    return(value & (1 << n_bit));
}

/* X Axis */
void moveXRight() {
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 6, 0);      // set bit 6 to low level
    setBitValue(&p, 7, 1);      // set bit 7 to high level
    writeDigitalU8(2, p);       // update port 2
}

void moveXLeft() {
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 6, 1);      // set bit 6 to high level
    setBitValue(&p, 7, 0);      // set bit 7 to low level
    writeDigitalU8(2, p);       // update port 2
}

void stopX() {
    uInt8 p = readDigitalU8(2); // read port 2
    setBitValue(&p, 6, 0);      // set bit 6 to low level
    setBitValue(&p, 7, 0);      // set bit 7 to low level
    writeDigitalU8(2, p);       // update port 2
}

int getXPos() {
    uInt8 p = readDigitalU8(0);
    if (!getBitValue(p, 2)) return 1;
    if (!getBitValue(p, 1)) return 2;
    if (!getBitValue(p, 0)) return 3;
    return(-1);
}

/* Z Axis */
void moveZUp() {
    uInt8 p = readDigitalU8(2);
    setBitValue(&p, 3, 1);
    setBitValue(&p, 2, 0);
    writeDigitalU8(2, p);
}

void moveZDown(){
    uInt8 p = readDigitalU8(2);
    setBitValue(&p, 3, 0);  
    setBitValue(&p, 2, 1);  
    writeDigitalU8(2, p);
}

void stopZ() {

    uInt8 p = readDigitalU8(2);
    setBitValue(&p, 3, 0);  
    setBitValue(&p, 2, 0);  
    writeDigitalU8(2, p);

}

int getZPos() {

    uInt8 p0 = readDigitalU8(0);
    uInt8 p1 = readDigitalU8(1);

    if (!getBitValue(p1, 3)) return 1; // position z = 1 bottom position 
    if (!getBitValue(p1, 2)) return 2; // position z = 2 
    if (!getBitValue(p1, 1)) return 3; // position z = 3 

    if (!getBitValue(p1, 0)) return 4; // position z = 4 
    if (!getBitValue(p0, 7)) return 5; // position z = 5 
    if (!getBitValue(p0, 6)) return 6; // position z = 6 top position
    return(-1);
}


/* Y Axis*/
void moveYInside() {
    uInt8 p = readDigitalU8(2);
    setBitValue(&p, 4, 0);
    setBitValue(&p, 5, 1);
    writeDigitalU8(2, p);
}

void moveYOutside() {
    uInt8 p = readDigitalU8(2); 
    setBitValue(&p, 4, 1); 
    setBitValue(&p, 5, 0); 
    writeDigitalU8(2, p); 
}

void stopY() {
    uInt8 p = readDigitalU8(2); 
    setBitValue(&p, 4, 0); 
    setBitValue(&p, 5, 0); 
    writeDigitalU8(2, p); 
}

int getYPos() {
    uInt8 p0 = readDigitalU8(0);
    if (!getBitValue(p0, 5)) return 1;
    if (!getBitValue(p0, 4)) return 2;
    if (!getBitValue(p0, 3)) return 3;
    return(-1);
}


/* Switches */
int getSwitch1() {
    uInt8 p = readDigitalU8(1);
    if ((getBitValue(p, 5)) && !getBitValue(p, 6))
        return 1;
    return 0;
}
int getSwitch2() {
    uInt8 p = readDigitalU8(1);
    if (getBitValue(p, 6) && !(getBitValue(p, 5)))
        return 1;
    return 0;
}

int getSwitch1_2() {
    uInt8 p = readDigitalU8(1);
    if (getBitValue(p, 6) && (getBitValue(p, 5)))
        return 1;
    return 0;
}

/* Leds */
void ledOn(int led) {
    uInt8 p = readDigitalU8(2);
    if (led == 1) {
        //setBitValue(&p, 1, 0);
        setBitValue(&p, 0, 1);
    }
    else if (led == 2) {
        setBitValue(&p, 1, 1);
        //setBitValue(&p, 0, 0);
    }
    else { 
        return;
    }
    writeDigitalU8(2, p);
}

/* Sensors */

int getPalleteSen() {
   
    return( (readDigitalU8(1) & 0b10000) >> 4);
}

void ledsOff() {
    uInt8 p = readDigitalU8(2);
    setBitValue(&p, 1, 0);
    setBitValue(&p, 0, 0);
    writeDigitalU8(2, p);
}

uInt8 ReadPort(int p) {

    if (p < 0 || p > 2) { return 0; }

    return readDigitalU8(p);
}

void WritePort(int p,uInt8 value) {

    if (p != 2 ) { return; }

    writeDigitalU8(p, value);
}
