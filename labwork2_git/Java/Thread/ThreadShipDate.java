import java.time.Duration;
import java.time.LocalDateTime;

public class ThreadShipDate extends Thread implements Threadx{

    Cell cell;
    boolean shipped;
    private boolean stop;
    private boolean alive;
    
    public ThreadShipDate(Cell c){
        
        Mechanism.thread_manager.AddThread(this);
        cell    = c;
        shipped = false;
        stop    = false;
        alive   = true;
    }

    public void Pause(){
        stop = true;
        this.interrupt();
    }

    public void UnPause(){
        stop = false;
    }

    public void Kill(){
        
        if(shipped){
            Mechanism.thread_led1.Off();
        }
        stop  = true;
        alive = false;
        this.interrupt();
    }

    private void signalLed1(){

        shipped = true;
        Mechanism.thread_led1.add();
    }



   @Override
    public void run(){

        while(alive){
            if(stop)
                continue;

            LocalDateTime now = LocalDateTime.now();

            Duration d = Duration.between(now,cell.date);
            
            if( d.getSeconds() < 0 ){
                signalLed1();
                return;
            }

            try {
                Thread.sleep(d.toMillis());
                signalLed1();
                return; 
            } catch (InterruptedException e) {
                //Thread got interrupted
                continue;
            }
        }   
    }
}