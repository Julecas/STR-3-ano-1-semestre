import java.util.concurrent.Semaphore;

public class ThreadCalibration extends Thread{
    private final Axis axis;
    private boolean runOnce;
    private Semaphore semCalibrate = null;

    public ThreadCalibration(Axis axis){//constructor
        this.axis = axis;
        this.runOnce = true;
        this.semCalibrate = new Semaphore(0);
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

        if(runOnce){
        axis.moveForward();
        while (axis.getPos() == -1) { } //stay in loop, se estiver em movimento
        axis.stop();
        runOnce = false;
        }
        
        if(axis.getPos() != 1){
            axis.gotoPos(1);
            while(axis.getPos() == -1){}
        }
    }
    

    @Override
    public void run(){
        while(true){
                this.initializeCalibration();
            }
    }

}
