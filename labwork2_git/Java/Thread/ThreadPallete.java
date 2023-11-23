
import java.util.concurrent.Semaphore;

public class ThreadPallete extends Thread{

    private final Axis axisZ;
    private final Axis axisY;
    private Semaphore semaphoreY = null;

    public ThreadPallete(Axis axisZ,Axis axisY){ //constructor
        this.axisZ = axisZ;
        this.axisY = axisY;
        this.semaphoreY = new Semaphore(0);
    }

    public void Pallete() throws InterruptedException{
        
        //this.semaphoreY.acquire();//wait for permission to move

        if(axisZ.getPos() % 2 == 0){ //place pallet 

            axisY.moveForward();
            while(axisY.getPos() != 3){} //move y front
            axisY.stop();

            axisZ.moveBackward();
            Thread.sleep(100);//ms
            while(axisZ.getPos() == -1){} //moe z down
            axisZ.stop();

            axisY.moveBackward();
            while(axisY.getPos() != 2){} //move y back
            axisY.stop();

            axisZ.moveForward();
            Thread.sleep(100);//ms
            while(axisY.getPos() == -1){} //moe z down
            axisZ.stop();
        } else { //get pallete
        
            axisY.moveBackward();
            while(axisY.getPos() != 1){} //move y front
            axisY.stop();   
        }
    }

    public Semaphore getSemaphoreY(){
        return this.semaphoreY;
    }
    
    @Override
    public void run(){
        try {
            this.Pallete();
        } catch (InterruptedException e) {
            System.out.println("Problema no semY");
        }
    }
}