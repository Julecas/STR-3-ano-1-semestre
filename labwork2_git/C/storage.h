#pragma once

# include <interface.h>

/* DAQ Initialization */
void initializeHardwarePorts();

/* X Axis */
void moveXRight();
void moveXLeft();
void stopX();
int getXPos();

/* Z Axis*/
void moveZUp();
void moveZDown();
void stopZ();
int getZPos();

/* Y Axis*/
void moveYInside();
void moveYOutside();
void stopY();
int getYPos();

/* Switches */
int getSwitch1();
int getSwitch2();
int getSwitch1_2();

/* Leds */
void ledOn(int led);
void ledsOff();

int getPalleteSen();
