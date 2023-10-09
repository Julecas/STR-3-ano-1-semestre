#include "my_interaction_functions.h"
#include <windows.h> //for Sleep function
#include <mongoose.h>


//DONNE

//NOTA:     MARTIM fiz os atuadores assim para n?o fazer 9 fun??es iguais (eles queriam coco basicamente)


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
	setBitValue(&p, 0 , 0); // set bit 0 to low level
	setBitValue(&p, 1, 1); // set bit 1 to high level
	writeDigitalU8(2, p); // update port 2
}

void moveCylinder1Front() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 3 , 0); // set bit 0 to low level
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
		while (getCylinderStartPos() != 0) {

			moveCylinderStartBack(); //cylinder 0
		}
		stopCylinderStart();
		return;
	}
	//front (end goal)
	if (pos == 1) {
		while (getCylinderStartPos() != 1) {

			moveCylinderStartFront(); //cylinder 0
		}
		stopCylinderStart();
		return;
	}

}

void gotoCylinder1(int pos) {

	//back (end goal)
	if (pos == 0) {
		while (getCylinder1Pos() != 0) {

			moveCylinder1Back(); //cylinder 1
		}
		stopCylinder1();
		return;
	}
	//front (end goal)
	if (pos == 1) {
		while (getCylinder1Pos() != 1) {

			moveCylinder1Front(); //cylinder 1
		}
		stopCylinder1();
		return;
	}

}

void gotoCylinder2(int pos) {

	//back (end goal)
	if (pos == 0) {
		while (getCylinder2Pos() != 0) {

			moveCylinder2Back(); //cylinder 2
		}
		stopCylinder2();
		return;
	}
	//front (end goal)
	if (pos == 1) {
		while (getCylinder2Pos() != 1) {

			moveCylinder2Front(); //cylinder 2
		}
		stopCylinder2();
		return;
	}

}


// put here all function's implementationss