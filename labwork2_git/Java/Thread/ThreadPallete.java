
import java.util.concurrent.Semaphore;

public class ThreadPallete extends Thread{

    private final Axis axisX;
    private final Axis axisZ;
    private final Axis axisY;
    private Semaphore semY = null;

    public ThreadPallete(Axis axisX ,Axis axisZ,Axis axisY){ //constructor
        this.axisZ = axisZ;
        this.axisY = axisY;
        this.axisX = axisX;
        this.semY = new Semaphore(0);
    }

    public Semaphore getSemaphoreY(){
        return this.semY;
    }

    public void SemaphoreYRelease(){
        semY.release();    
    }

    public void Pallete() throws InterruptedException{
        

        if(axisZ.getPos() != -1 && axisX.getPos() != -1){//move only when in position 1,1

            this.semY.acquire();//wait for permission to move

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
    }
    
    @Override
    public void run(){
        while(true){
            try {
                this.Pallete();
            } catch (InterruptedException e) {
                System.out.println("Erro thread pallete" + e);
            }
        }
    }
}