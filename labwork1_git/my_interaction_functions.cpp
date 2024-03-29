#include "my_interaction_functions.h"
#include <mongoose.h>
#include <stdio.h>
#include <conio.h>

//debug
#include <bitset>
#include <iostream>

//debug

extern "C" {
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <interface.h>	
#include <interrupts.h>
}


void Conveyor(bool b);
void moveCylinder(int port, int bitF, bool Fv, int bitB, bool Fb);



void senseBlockCylinder2() {

	uInt8 p1;

	while (TRUE) {

		p1 = readDigitalU8(1); // read port 1

		if (getBitValue(p1, 7)) {  //get bit 7 active high
			return;
		}
	}
}

void senseBlockCylinder1() {

	while (TRUE) {

		if (senseBlockCylinder1value()) {
			return;
		}
	}
}

bool senseBlockCylinder1value() {

	//Return true if block is under sensor
	return readDigitalU8(0) & 1 ? true : false;

}

void ledRejectOn() {

	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 7, 1);

	taskENTER_CRITICAL();
	writeDigitalU8(2, p);
	taskEXIT_CRITICAL();
}

void ledRejectOff() {

	uInt8 p = readDigitalU8(2); // read port 2

	setBitValue(&p, 7, 0);
	taskENTER_CRITICAL();

	writeDigitalU8(2, p);
	taskEXIT_CRITICAL();
}

void cylinderStartFrontBack() {

	gotoCylinderStart(1);
	gotoCylinderStart(0);
}


void cylinder1FrontBack() {

	gotoCylinder1(1);
	gotoCylinder1(0);
}

void cylinder2FrontBack() {

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

	moveCylinder(2, 5, 0, 6, 0); //aqui
}

void moveCylinderStartBack() {

	moveCylinder(2, 1, 0, 0, 1);
}

void moveCylinder1Back() {

	moveCylinder(2, 4, 0, 3, 1);
}

void moveCylinder2Back() {

	moveCylinder(2, 6, 0, 5, 1);
}

void moveCylinderStartFront() {

	moveCylinder(2, 0, 0, 1, 1);
}

void moveCylinder1Front() {

	moveCylinder(2, 3, 0, 4, 1);

}
void moveCylinder2Front() {

	moveCylinder(2, 5, 0, 6, 1);

}

//moveCylinder(2, 0, 0, 1, 0);
void moveCylinder(int port, int bitF, bool Fv, int bitB, bool Fb) {

	uInt8 p = readDigitalU8(port); // read port 
	setBitValue(&p, bitF, Fv);	  // Move front 
	setBitValue(&p, bitB, Fb);   // Move back
	
	taskENTER_CRITICAL();
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

	if (pos == 1) {
		moveCylinder2Front(); //cylinder 2

		while (getCylinder2Pos() != 1) {
			continue;
		}
		stopCylinder2();
		return;
	}

}


void ConveyorOn() {
	Conveyor(true);
}

void ConveyorOff() {
	Conveyor(false);
}

//True	on
//False off
void Conveyor(bool b) {

	//task critic
	uInt8 p = readDigitalU8(2); // read port 2
	setBitValue(&p, 2, b);

	taskENTER_CRITICAL();
	writeDigitalU8(2, p);
	taskEXIT_CRITICAL();

}


uInt8 ReadTypeValue(){

	uInt8 p1,
		c = 0;

	moveCylinderStartFront(); //cylinder 0

	while (getCylinderStartPos() != 1) {

		p1 = readDigitalU8(1);
		p1 &= 0b01100000;
		c |= p1;
		//printf("lido %d\n", ((c & 0b00100000) > 0) + (c >> 6));


	}
	vTaskDelay(50);

	return
		((c & 0b00100000) > 0) + (c >> 6);//counts how many bits == 1

}


void cylinderTest() {
	int tecla = 0;
	std::cout << "\nComandos :\n";
	std::cout << "cylinder Start frente q\n";
	std::cout << "cylinder Start tras   a\n";
	std::cout << "cylinder 1     frente w\n";
	std::cout << "cylinder 1     tras   s\n";
	std::cout << "cylinder 2     frente e\n";
	std::cout << "cylinder 2     tras   d\n";
	std::cout << "Sair p\n";
	while (TRUE) {

		tecla = _getch();
		switch (tecla) {
		case 'q': {gotoCylinderStart(1); 	break; }
		case 'a': {gotoCylinderStart(0);	break; }
		case 'w': {gotoCylinder1(1);		break; }
		case 's': {gotoCylinder1(0); 		break; }
		case 'e': {gotoCylinder2(1);		break; }
		case 'd': {gotoCylinder2(0);		break; }
		case 'p':
			system("cls");
			return;
			//default: return;
		}
	}
}


// put here all function's implementationss