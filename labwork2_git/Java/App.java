import java.util.Scanner;
//import java.util.concurrent.ArrayBlockingQueue;
//import java.util.concurrent.Semaphore;

public class App {

    private static AxisZ axisZ;
    private static AxisX axisX;
    //private static AxisY axisY;
    private static ThreadCalibration thread_calibrateZ;
    private static ThreadCalibration thread_calibrateX;
    private static ThreadGoto thread_gotoX;
    private static ThreadGoto thread_gotoZ;
    //private static ThreadPallete thread_gotoY; 
    private static int posX = 0;
    private static int posZ = 0;
    private static int op = -1;

    public static void main(String[] args) throws Exception {

        posX = 0;
        posZ = 0;
        op = -1;
        axisZ = new AxisZ();
        axisX = new AxisX();
        //axisY = new AxisY();
        thread_gotoX = new ThreadGoto(axisX);
        thread_gotoZ = new ThreadGoto(axisZ);
        thread_calibrateZ = new ThreadCalibration(axisZ);
        thread_calibrateX = new ThreadCalibration(axisX);
        //thread_gotoY = new ThreadPallete(axisZ, axisY);

        System.out.println("Labwork2 from Java");
        Storage.initializeHardwarePorts();
  
        menu();
    }

    public static void menu(){
       
        Scanner scan = new Scanner(System.in);

        thread_gotoX.start();
        thread_gotoZ.start();
        
        while(true){

            System.out.println("Enter an option");
            op = scan.nextInt();
            if( op == 1000){break;}
            if(op != 1){
                
                posZ = firstDigit(op);
                posX = lastDigit(op);

                System.out.println("Bf,mbx PosZ =" + posZ);
                System.out.println("Bf,mbx PosX =" + posX);


                thread_gotoZ.AddQueue(posZ);
                thread_gotoX.AddQueue(posX);
                continue;
            }
            calibrate( thread_calibrateX, thread_calibrateZ); 
            
        }
        scan.close();

    }

    public static int firstDigit(int op){
        // To find the last digit of the number : taking modulo with 10
        return  Math.abs(op % 10);
    }
     public static int lastDigit(int op){
        // To find the last digit of the number
        int tm = 0; 
        while (op != 0) {
            tm = Math.abs(op % 10);
            op /= 10;
        }
        return tm;
    }
    public static void calibrate(Thread thread_calibrateX,Thread thread_calibrateZ){
        thread_calibrateX.start();
        thread_calibrateZ.start();
        return;
    }
}
