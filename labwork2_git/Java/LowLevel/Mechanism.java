import java.util.concurrent.Semaphore;

public class Mechanism {
    
    public static AxisZ axisZ;
    public static AxisX axisX;
    public static AxisY axisY;
    private ThreadCalibration thread_calibrateZ;
    private ThreadCalibration thread_calibrateX;
    private ThreadGoto thread_gotoX;
    private ThreadGoto thread_gotoZ;
    private ThreadGoto thread_gotoY; 
    public  static ThreadManager thread_manager;
    public  static ThreadLed1 thread_led1;
    private static Semaphore semX; 
    private static Semaphore semY; 
    private static Semaphore semZ;
    
    public Mechanism(){
        
        //start variables
        semX                = new Semaphore(0);
        semZ                = new Semaphore(0);
        semY                = new Semaphore(0);
        axisX               = new AxisX();
        axisY               = new AxisY();
        axisZ               = new AxisZ();
        thread_gotoX        = new ThreadGoto(axisX);
        thread_gotoZ        = new ThreadGoto(axisZ);
        thread_gotoY        = new ThreadGoto(axisY);
        thread_calibrateZ   = new ThreadCalibration(axisZ);
        thread_calibrateX   = new ThreadCalibration(axisX);
        thread_led1         = new ThreadLed1();
 
        //start threads
        Goto(thread_gotoX,thread_gotoY,thread_gotoZ);
        calibrate(thread_calibrateX, thread_calibrateZ); 
        thread_led1.start();

    }

    public void calibrate(Thread thread_calibrateX,Thread thread_calibrateZ){
        thread_calibrateX.start();
        thread_calibrateZ.start();
        return;
    }

    public void Goto(Thread thread_gotoX,Thread thread_gotoY,Thread thread_gotoZ){
        thread_gotoX.start();
        thread_gotoY.start();
        thread_gotoZ.start();
        return;
    }

    public void autoCalibrate(){
        thread_calibrateX.SemaphoreCalibrateRelease();
        thread_calibrateZ.SemaphoreCalibrateRelease();
    }
    
    
    public static void ledOn(int ledNumber){
        Storage.ledOn(ledNumber);
    }

    public static void ledsOff(){
        Storage.ledsOff();
    }

    public static boolean switch1Pressed(){

        return Storage.getSwitch1() == 1;

    }

    public static boolean switch2Pressed(){
        //todo for now returns false
        return false;
    }

    public static boolean bothSwithcesPressed(){
        //todo for now returns false
        return false;
    }

    public static int getPalleteSen(){
        
        return Storage.getPalleteSen();
    }

    public boolean checkCell(int posZ, int posX) throws InterruptedException{
        
        posZ *= 2;
        //goto X,Z
        thread_gotoZ.AddQueue(posZ);
        thread_gotoX.AddQueue(posX);

        semX.acquire();
        semZ.acquire();

        thread_gotoZ.AddQueue(posZ-1);//go down Z
        semZ.acquire();

        thread_gotoY.AddQueue(3);//go to front Y
        semY.acquire();

        thread_gotoZ.AddQueue(posZ);//go down Z
        semZ.acquire();

        thread_gotoY.AddQueue(2);//go back Y
        semY.acquire();


        if(getPalleteSen() == 1){
            //está ocupado (arrumar caixa depois de verificar)
            thread_gotoY.AddQueue(3);//go to front Y
            semY.acquire();

            thread_gotoZ.AddQueue(posZ-1);//go down Z
            semZ.acquire();

            thread_gotoY.AddQueue(2);//go back Y
            semY.acquire();

            thread_gotoZ.AddQueue(posZ);
            semZ.acquire();
            return true;
        } else{
            //está livre
            return false;
        }
    }

    public boolean putPartInCell(int posZ, int posX) throws InterruptedException{
        
        posZ *= 2;
        //goto X,Z
        thread_gotoZ.AddQueue(posZ);
        thread_gotoX.AddQueue(posX);

        //ESPERAR por X,Z
    
        semX.acquire();
        semZ.acquire();
        
        //put part in cell
        thread_gotoY.AddQueue(3);//go to front Y
        semY.acquire();

        thread_gotoZ.AddQueue(posZ-1);//go down Z
        semZ.acquire();

        thread_gotoY.AddQueue(2);//go back Y
        semY.acquire();

        thread_gotoZ.AddQueue(posZ);
        semZ.acquire();

        return true;
    }

    public boolean takePartFromCell(int posZ, int posX) throws InterruptedException{
        
        posZ *= 2;
        
        //goto X,Z
        thread_gotoZ.AddQueue(posZ);
        thread_gotoX.AddQueue(posX);

        //ESPERAR por X,Z
    
        semX.acquire();
        semZ.acquire();
        
        //put part in cell
        thread_gotoZ.AddQueue(posZ-1);//go down Z
        semZ.acquire();

        thread_gotoY.AddQueue(3);//go to front Y
        semY.acquire();

        thread_gotoZ.AddQueue(posZ);
        semZ.acquire();

        thread_gotoY.AddQueue(2);//go back Y
        semY.acquire();

        return true;
    }

    public void linguarudo() throws InterruptedException{
        thread_gotoY.AddQueue(2);//go front Y
        semY.acquire();
    }

    public void GotoReference() throws InterruptedException{
       
        //goto X,Z
        thread_gotoZ.AddQueue(1);
        thread_gotoX.AddQueue(1);
        
        //ESPERAR por X,Z
        semX.acquire();
        semZ.acquire();

        thread_gotoY.AddQueue(1);//go back Y
        semY.acquire();
    }
    
    public void manualCalibrate(int posZ, int posX) throws InterruptedException{

        posZ *= 2;
        //goto X,Z
        thread_gotoZ.AddQueue(posZ);
        thread_gotoX.AddQueue(posX);

        //ESPERAR por X,Z

        semX.acquire();
        semZ.acquire();
    }

    public static void releaseSemZ(){
        semZ.release();
    }

    public static void releaseSemX(){
        semX.release();
    }

    public static void releaseSemY(){
        semY.release();
    }

}