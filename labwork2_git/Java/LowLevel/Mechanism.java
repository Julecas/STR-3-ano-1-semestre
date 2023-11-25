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
    private static Semaphore semX; 
    private static Semaphore semY; 
    private static Semaphore semZ;
    
    public Mechanism(){
        
        //start variables
        semX = new Semaphore(0);
        semZ = new Semaphore(0);
        semY = new Semaphore(0);
        axisX = new AxisX();
        axisY = new AxisY();
        axisZ = new AxisZ();
        thread_gotoX = new ThreadGoto(axisX);
        thread_gotoZ = new ThreadGoto(axisZ);
        thread_gotoY = new ThreadGoto(axisY);
        thread_calibrateZ = new ThreadCalibration(axisZ);
        thread_calibrateX = new ThreadCalibration(axisX);

         
        //start threads
         Goto(thread_gotoX,thread_gotoY,thread_gotoZ);
         calibrate(thread_calibrateX, thread_calibrateZ); 
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
    
    
    public void ledOn(int ledNumber){
        Storage.ledOn(ledNumber);
    }

    public void ledsOff(){
        Storage.ledsOff();
    }

    public boolean switch1Pressed(){
        //todo for now returns false
        if(Storage.getSwitch1() == 1){
            return true;
        }
        return false;
    }

    public boolean switch2Pressed(){
        //todo for now returns false
        return false;
    }

    public boolean bothSwithcesPressed(){
        //todo for now returns false
        return false;
    }

    public int getPalleteSen(){
        
        return Storage.getPalleteSen();
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