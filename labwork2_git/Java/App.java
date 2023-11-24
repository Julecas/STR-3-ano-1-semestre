import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Scanner;
import java.lang.Math;
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
    private static Stock stk;
    private static DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd/MM/yy HH:mm:ss");
    
    
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
        stk = new Stock();
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
    
    private void AddItem(Scanner scanner){

        int z,x,ref;
        String LoadName,aux,input;
        LocalDateTime ShipDate;
 
        //Perguntar coordenadas
        while(true){

            System.out.println("Insert the coordinates(x,z) or \'q\' to quit: ");
            
            input = scanner.nextLine();

            if( input.charAt(0) == 'q' ){
                return;
            }

            aux = input.substring(0, input.indexOf(','));

            try{
                x = Integer.parseInt(aux);
            }catch(Exception e){
                System.out.println("Invalid input(x)!\n"+ e);
                continue;
            }
 
            aux = input.substring(input.indexOf(','));

            try{
                z = Integer.parseInt(aux);
            }catch(Exception e){
                System.out.println("Invalid input(z)!\n"+ e);
                continue;
            }
            break;
        }
        //Perguntar data
        while(true){
            
            System.out.println("Insert Shipment date(DD/MM/YY hh:mm:ss) or \'q\' to quit:");
            input = scanner.nextLine();

            try {
                ShipDate = LocalDateTime.parse(input,formatter);
            } catch (Exception e) {
                System.out.println("Invalid input(z)!\n"+ e);
                continue;
            }
            break;
        }
        
        //load name
        System.out.println("Insert Load Name or \'q\' to quit: ");
        LoadName = scanner.nextLine();
        
        //reference code
        while(true){
            System.out.println("Insert reference code or \'q\' to quit: ");
            input = scanner.nextLine();

            try{
                ref = Integer.parseInt(input);
            }catch(Exception e){
                System.out.println("Invalid input(reference)!\n"+ e);
                continue;
            }
            break;
        }
 
        int res = stk.Add_item(LoadName, ref, ShipDate, z, x);

        switch (res) {
            case -2:
                //Pos errada
                break;
        
            case -1:
                //Pos Ocupada
                break;
            default:
                //Iniciar operation
                break;
        }
    }

}
