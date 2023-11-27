import java.util.concurrent.Semaphore;

public class ThreadLed1 extends Thread implements Threadx{

    private Semaphore SemStart;
    private int       counting;
    private boolean   TurnOff;    
    private boolean   alive;
    private boolean   stop;

    public ThreadLed1(){

        Mechanism.thread_manager.AddThread(this);
        this.TurnOff  = false;
        this.SemStart = new Semaphore(0);
        counting      = 0;
        alive         = true;
        stop          = false;
    }
 
    public void Off(){
        this.TurnOff = true;
        this.interrupt();
    }

    public void add(){

        if( counting < Stock.cap ){
            ++counting;
            this.SemStart.release();
        }
    }
 
    @Override
    public void run(){
        
        while(alive){

            if( stop )
                continue;

            try {
                this.SemStart.acquire();
                --counting;
                this.TurnOff = false;
            } catch (InterruptedException e) {
                continue;
            }

            while( !this.TurnOff ){

                 try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    Mechanism.ledsOff();
                }

                Mechanism.ledOn(1);
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    Mechanism.ledsOff();
                }
                Mechanism.ledsOff(); 
            }
        }
    }

    @Override
    public void Pause() {
        stop = true;
        this.interrupt();
    }

    @Override
    public void UnPause() {
        stop = false;
    }

    @Override
    public void Kill() {
        stop  = true;
        alive = false;
        this.interrupt();
    }
}
