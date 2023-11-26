import java.util.concurrent.Semaphore;

public class ThreadLed1 extends Thread {

    private Semaphore SemStart;
    private int       counting;
    private boolean   TurnOff;

    public ThreadLed1(){
        this.TurnOff  = false;
        this.SemStart = new Semaphore(0);
        counting      = 0;
    }
 
    public void Off(){
        this.TurnOff = true;
        System.out.println(TurnOff);
    }

    public void add(){

        if( counting < Stock.cap ){
            ++counting;
            this.SemStart.release();
        }
    }
 
    @Override
    public void run(){
        
        while(true){

            try {
                this.SemStart.acquire();
                --counting;
                this.TurnOff = false;
            } catch (InterruptedException e) {
                System.out.println("Error Trying to take semaphore on ThreadLed1"+e);
            }

            while( !this.TurnOff ){

                 try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                }

                Mechanism.ledOn(1);
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                }
                Mechanism.ledsOff();
                
            }
        }
    }
}
