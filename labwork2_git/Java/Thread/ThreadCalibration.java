import java.util.concurrent.Semaphore;

public class ThreadCalibration extends Thread implements Threadx{
    
    private final Axis axis;
    //private boolean runOnce;
    private Semaphore semCalibrate;
    private boolean stop;
    private boolean alive;

    public void Pause(){
        stop = true;
        axis.Pause();
    }

    public void UnPause(){
        stop = false;
        axis.UnPause();
    }

    public void Kill(){
        stop  = true;
        axis.Kill();
        alive = false;
    }

    public ThreadCalibration(Axis axis){//constructor

        Mechanism.thread_manager.AddThread(this);
        this.axis = axis;
        //this.runOnce = true;
        this.semCalibrate = new Semaphore(0);
        stop = false;
        alive = true;
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
            while (axis.getPos() == -1 && !stop ) { } //stay in loop, se estiver em movimento
            axis.stop();
        }
        
        if(axis.getPos() != 1){
            axis.gotoPos(1);
        }
    }
    

    @Override
    public void run(){
        while(alive){
        
            if( !stop )
                this.initializeCalibration();
        }
    }

}
