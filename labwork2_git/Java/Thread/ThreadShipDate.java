import java.time.Duration;
import java.time.LocalDateTime;

public class ThreadShipDate extends Thread{

    Cell cell;
    
    public ThreadShipDate(Cell c){
        cell = c;
    }

   @Override
    public void run(){
        LocalDateTime now = LocalDateTime.now();

        Duration d = Duration.between(cell.date,now);

        if( d.getSeconds() < 0 ){
            return;
        }
        
        try {
            Thread.sleep(d.toMillis());
        } catch (InterruptedException e) {
            //Thread got interrupted
            return;
        }
        
        ThreadLed1.sem.release();
        
    }
}
