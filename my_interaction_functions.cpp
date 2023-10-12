#include "my_interaction_functions.h"
#include <mongoose.h>


void calibrateCylinder1() {

	gotoCylinder1(1);
	gotoCylinder1(0);
}

void calibrateCylinder2() {

	gotoCylinder2(1);
	gotoCylinder2(0);

}


void stopCylinderStart() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 0, 0); // set bit 0 to low level
	setBitValue(&p, 1, 0); // set bit 1 to low level
	writeDigitalU8(2, p); // update port 2
}

void stopCylinder1() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 3, 0); // set bit 0 to low level
	setBitValue(&p, 4, 0); // set bit 1 to low level
	writeDigitalU8(2, p); // update port 2
}

void stopCylinder2() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 5, 0); // set bit 0 to low level
	setBitValue(&p, 6, 0); // set bit 1 to low level
	writeDigitalU8(2, p); // update port 2
}

void moveCylinderStartBack() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 1, 0); // set bit 1 to low level
	setBitValue(&p, 0, 1); // set bit 0 to high level
	writeDigitalU8(2, p); // update port 2
}

void moveCylinder1Back() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 4, 0); // set bit 1 to low level
	setBitValue(&p, 3, 1); // set bit 0 to high level
	writeDigitalU8(2, p); // update port 2
}

void moveCylinder2Back() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 6, 0); // set bit 1 to low level
	setBitValue(&p, 5, 1); // set bit 0 to high level
	writeDigitalU8(2, p); // update port 2
}

void moveCylinderStartFront() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 0, 0); // set bit 0 to low level
	setBitValue(&p, 1, 1); // set bit 1 to high level
	writeDigitalU8(2, p); // update port 2
}

void moveCylinder1Front() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 3, 0); // set bit 0 to low level
	setBitValue(&p, 4, 1); // set bit 1 to high level
	writeDigitalU8(2, p); // update port 2
}
void moveCylinder2Front() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 5, 0); // set bit 0 to low level
	setBitValue(&p, 6, 1); // set bit 1 to high level
	writeDigitalU8(2, p); // update port 2
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
		while (getCylinder1Pos() != 1){
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