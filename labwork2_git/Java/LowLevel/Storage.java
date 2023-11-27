
public class Storage {

    static{     //c:\STR\Labwork2\storage\x64\Debug\storage.dll
        System.load("C:\\STR\\Labwork2\\storage\\x64\\Debug\\storage.dll");
    }

    /* DAQ Initialization */
    static synchronized native void initializeHardwarePorts();

    /* X Axis */
    static synchronized native void moveXRight();
    static synchronized native void moveXLeft();
    static synchronized native void stopX();
    static synchronized native int getXPos();

    /* Z Axis*/
    static synchronized native void moveZUp();
    static synchronized native void moveZDown();
    static synchronized native void stopZ();
    static synchronized native int getZPos();

    /* Y Axis*/
    static synchronized native void moveYInside();
    static synchronized native void moveYOutside();
    static synchronized native void stopY();
    static synchronized native int getYPos();

    /* Switches */
    static synchronized native int getSwitch1();
    static synchronized native int getSwitch2();
    static synchronized native int getSwitch1_2();

    /* Leds */
    static synchronized native void ledOn(int led);
    static synchronized native void ledsOff();

    /* sensors */
    static synchronized native int getPalleteSen();

    static synchronized native byte ReadPort(int p);
    static synchronized native void WritePort(int p, byte value);

}
