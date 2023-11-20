import java.util.Scanner;

public class App {
    public static void main(String[] args) throws Exception {
        System.out.println("Labwork2 from Java");

        Storage.initializeHardwarePorts();

        //calibration**********************************************************
        AxisZ axisZ = new AxisZ();
        AxisX axisX = new AxisX();
        Thread calibrateZ = new CalibrationThreadZ(axisZ);
        Thread calibrateX = new CalibrationThreadX(axisX);
        //**************************************************************

        int op = -1;
        Scanner scan = new Scanner(System.in);

        while(op != 0){
            System.out.println("Enter an option");
            op = scan.nextInt();
            switch(op){
                case 1: calibrateX.start();calibrateZ.start(); break;   //calibration start
             
                default: break;
            }
        }
        scan.close();
    }
}
