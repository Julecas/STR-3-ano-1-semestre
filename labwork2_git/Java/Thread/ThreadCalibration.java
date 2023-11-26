import java.util.concurrent.Semaphore;

public class ThreadCalibration extends Thread{
    
    private final Axis axis;
    private Semaphore semCalibrate;
    private boolean stop;

    public ThreadCalibration(Axis axis){//constructor

        this.axis = axis;
        this.semCalibrate = new Semaphore(0);
        stop = false;
    }

    public void kill(){
        stop = true;
    }

    public void SemaphoreCalibrateRelease(){
        semCalibrate.release();    
    }

    public void initializeCalibration(){

        try {
            this.semCalibrate.acquire(); //sem
        } catch (InterruptedException e) {
            System.out.println("Erro" + e);
        }

        if(axis.getPos() == -1){
        axis.moveForward();
        while (axis.getPos() == -1) { } //stay in loop, se estiver em movimento
        axis.stop();
        }
        
        if(axis.getPos() != 1){
            axis.gotoPos(1);
            while(axis.getPos() == -1){}
        }
    }
    

    @Override
    public void run(){
        while(!stop){
                this.initializeCalibration();
            }
    }

}
