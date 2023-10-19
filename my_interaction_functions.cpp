#include "my_interaction_functions.h"
#include <mongoose.h>

void Conveyor(bool b);


void calibrateCylinder1() {

	gotoCylinder1(1);
	gotoCylinder1(0);
}

void calibrateCylinder2() {

	gotoCylinder2(1);
	gotoCylinder2(0);

}


void stopCylinderStart() {

	moveCylinder(2, 0, 0, 1, 0);
}

void stopCylinder1() {

	moveCylinder(2, 3, 0, 4, 0);
}

void stopCylinder2() {

	moveCylinder( 1,5,0,6,0 );
}

void moveCylinderStartBack() {

	moveCylinder(2, 1, 0, 0, 1);
}

void moveCylinder1Back() {

	moveCylinder(2, 4, 0, 3, 1);
}

void moveCylinder2Back() {

	moveCylinder(2,6,0,5,1);
}

void moveCylinderStartFront() {

	moveCylinder(2,0,0,1,1);
}

void moveCylinder1Front() {

	moveCylinder( 2,3,0,4,1);

}
void moveCylinder2Front() {
	
	moveCylinder(2,5,0,6,1);

}


void moveCylinder(int port,int bitF,bool Fv,int bitB,bool Fb) {

	taskENTER_CRITICAL();
	uInt8 p = readDigitalU8(port); // read port 
	setBitValue(&p, bitF, Fv);	  // Move front 
	setBitValue(&p, bitB, Fb);   // Move back
	writeDigitalU8(port, p);    // update port
	taskEXIT_CRITICAL();
}

int getCylinderStartPos() {
	//cylinder 0 sensor
	uInt8 p0 = readDigitalU8(0);
	if (getBitValue(p0, 6)) //back
		return 0;
	if (getBitValue(p0, 5)) //front
		return 1;
	return(-1);
}
int getCylinder1Pos() { //DONNE
	//cylinder 1 sensor
	uInt8 p0 = readDigitalU8(0);
	if (!getBitValue(p0, 4)) //back
		return 0;
	if (!getBitValue(p0, 3))  //front
		return 1;
	return(-1);

}
int getCylinder2Pos() {

	uInt8 p0 = readDigitalU8(0);
	if (!getBitValue(p0, 2)) //back
		return 0;
	if (!getBitValue(p0, 1)) //front
		return 1;
	return(-1);
}
int getBitValue(uInt8 value, uInt8 bit_n) {

	return(value & (1 << bit_n));
}
void setBitValue(uInt8* variable, int n_bit, int new_value_bit) {

	uInt8 mask_on = (uInt8)(1 << n_bit);
	uInt8 mask_off = ~mask_on;
	if (new_value_bit) *variable |= mask_on;
	else *variable &= mask_off;
}

void gotoCylinderStart(int pos) {

	//back (end goal)
	if (pos == 0) {

		moveCylinderStartBack(); //cylinder 0
		while (getCylinderStartPos() != 0) {
			continue;
		}
		stopCylinderStart();
		return;
	}
	//front (end goal)
	if (pos == 1) {

		moveCylinderStartFront(); //cylinder 0
		while (getCylinderStartPos() != 1) {
			continue;
		}
		stopCylinderStart();
		return;
	}

}

void gotoCylinder1(int pos) {

	//back (end goal)
	if (pos == 0) {

		moveCylinder1Back(); //cylinder 1
		while (getCylinder1Pos() != 0) {
			continue;
		}
		stopCylinder1();
		return;
	}
	//front (end goal)
	if (pos == 1) {

		moveCylinder1Front(); //cylinder 1
		while (getCylinder1Pos() != 1) {
			continue;
		}
		stopCylinder1();
		return;
	}

}

void gotoCylinder2(int pos) {

	//back (end goal)
	if (pos == 0) {

		moveCylinder2Back(); //cylinder 2
		while (getCylinder2Pos() != 0) {
			continue;
		}
		stopCylinder2();
		return;
	}
	//front (end goal)

	moveCylinder2Front(); //cylinder 2
	if (pos == 1) {
		while (getCylinder2Pos() != 1) {
			continue;
		}
		stopCylinder2();
		return;
	}

}


// put here all function's implementationss


void ConveyorOn(){
	Conveyor(true);
}

void ConveyorOff() {
	Conveyor(false);
}

//True	on
//False off
void Conveyor(bool b) {

	//int n = b ? 1 : 0;
	//task critic
	taskENTER_CRITICAL();
	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 2, b); 
	writeDigitalU8(2, p);
	taskEXIT_CRITICAL();

}

uInt8 ReadTypeBlock(){

	uInt8 p1,
		  c  = 0,
		  p2 = readDigitalU8(0);

	while ( p2 | 0b11011111 ) {

		p2 = readDigitalU8(0);
		p1 = readDigitalU8(1);
		p1 &= 0b01100000;
		c  |= p1;

	}
	return c;

}

// put here all function's implementationss
