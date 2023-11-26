
#include <jni.h>
#include <storage.h>

	JNIEXPORT int JNICALL Java_Storage_getPalleteSen(JNIEnv* jnienv, jclass jclass) {
		getPalleteSen();
	}

	JNIEXPORT void JNICALL Java_Storage_initializeHardwarePorts(JNIEnv* jnienv, jclass jclass){
		initializeHardwarePorts();
	}


	JNIEXPORT void JNICALL Java_Storage_moveXRight(JNIEnv* jniEnv, jclass jclass){
		moveXRight();
	}


	JNIEXPORT void JNICALL Java_Storage_moveXLeft(JNIEnv* jniEnv, jclass jclass) {
		moveXLeft();
	}


	JNIEXPORT void JNICALL Java_Storage_stopX(JNIEnv* jniEnv, jclass jclass) {
		stopX();
	}


	JNIEXPORT jint JNICALL Java_Storage_getXPos(JNIEnv* jniEnv, jclass jclass) {
		return getXPos();
	}


	JNIEXPORT void JNICALL Java_Storage_moveZUp(JNIEnv* jniEnv, jclass jclass) {
		moveZUp();
	}


	JNIEXPORT void JNICALL Java_Storage_moveZDown(JNIEnv* jniEnv, jclass jclass) {
		moveZDown();
	}

	
	JNIEXPORT void JNICALL Java_Storage_stopZ(JNIEnv* jniEnv, jclass jclass) {
		stopZ();
	}


	JNIEXPORT jint JNICALL Java_Storage_getZPos(JNIEnv* jniEnv, jclass jclass) {
		return getZPos();
	}

	
	JNIEXPORT void JNICALL Java_Storage_moveYInside(JNIEnv* jniEnv, jclass jclass) {
		moveYInside();
	}

	
	JNIEXPORT void JNICALL Java_Storage_moveYOutside(JNIEnv* jniEnv, jclass jclass) {
		moveYOutside();
	}

	
	JNIEXPORT void JNICALL Java_Storage_stopY(JNIEnv* jniEnv, jclass jclass) {
		stopY();
	}

	
	JNIEXPORT jint JNICALL Java_Storage_getYPos(JNIEnv* jniEnv, jclass jclass) {
		return getYPos();
	}


	JNIEXPORT jint JNICALL Java_Storage_getSwitch1(JNIEnv* jniEnv, jclass jclass) {
		return getSwitch1();
	}

	
	JNIEXPORT jint JNICALL Java_Storage_getSwitch2(JNIEnv* jniEnv, jclass jclass) {
		return getSwitch2();
	}

	
	JNIEXPORT jint JNICALL Java_Storage_getSwitch1_12(JNIEnv* jniEnv, jclass jclass) {
		return getSwitch1_2();
	}

	
	

	JNIEXPORT void JNICALL Java_Storage_ledOn(JNIEnv* jniEnv, jclass jclass, jint n) {
		ledOn(n);
	}

	
	JNIEXPORT void JNICALL Java_Storage_ledsOff(JNIEnv* jniEnv, jclass jclass) {
		ledsOff();
	}
