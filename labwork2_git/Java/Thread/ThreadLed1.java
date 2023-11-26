import java.util.concurrent.Semaphore;

public class ThreadLed1 extends Thread {

    public static Semaphore sem = new Semaphore(Stock.cap);


    @Override
    public void run(){
        while(true){
            try {
                sem.acquire();
            } catch (InterruptedException e) {
                System.out.println("Error Trying to take mbx on ThreadLed1"+e);
            }

            //TODO: piscar o led

        }
    }
    
}
