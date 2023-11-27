import java.time.Duration;
import java.time.LocalDateTime;

public class ThreadShipDate extends Thread{

    Cell cell;
    boolean shipped;
    //ThreadLed1 TLed;
    
    public ThreadShipDate(Cell c){
        cell = c;
        shipped = false;
        //TLed = Mechanism.thread_led1;
    }

    private void signalLed1(){

        shipped = true;
        Mechanism.thread_led1.add();
    }

    public void kill(){
        
        if(shipped){
            Mechanism.thread_led1.Off();
        }
        this.stop();

    }

   @Override
    public void run(){
        LocalDateTime now = LocalDateTime.now();

        Duration d = Duration.between(now,cell.date);
        
        if( d.getSeconds() < 0 ){
            signalLed1();
            return;
        }
        
        try {
            Thread.sleep(d.toMillis());
        } catch (InterruptedException e) {
            //Thread got interrupted
            return;
        }
        
        signalLed1();        
    }
}