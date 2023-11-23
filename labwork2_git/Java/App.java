import java.util.Scanner;

import Axis.AxisX;
import Axis.AxisZ;
import LowLevel.Storage;


public class App {
    public static void main(String[] args) throws Exception {

        int posX = 0;
        int posZ = 0;

        System.out.println("Labwork2 from Java");
        Storage.initializeHardwarePorts();

        //calibration**********************************************************
        AxisZ axisZ = new AxisZ();
        AxisX axisX = new AxisX();
        Thread thread_calibrateZ = new CalibrationThreadZ(axisZ);
        Thread thread_calibrateX = new CalibrationThreadX(axisX);
        //**************************************************************
        Thread thread_gotoX; 
        Thread thread_gotoZ; 


        int op = -1;
        Scanner scan = new Scanner(System.in);

        while(op != 0){
            
            System.out.println("Enter an option");
            op = scan.nextInt();
            
            if(op != 1){

                // To find the last digit of the number : taking modulo with 10
                posZ = Math.abs(op % 10);

                // To find the first digit of the number
                while (op != 0) {
                    posX = Math.abs(op % 10);
                    op /= 10;
                 }
            }

            thread_gotoX = new threadGoto(axisZ, posZ);
            thread_gotoZ = new threadGoto(axisX, posX);

            switch(op){
                case 1: thread_calibrateX.start();thread_calibrateZ.start(); break;   //calibration start
                //test
                default: thread_gotoX.start(); thread_gotoZ.start(); break;
            }
        }
        scan.close();
    }

}
