public class ThreadLedStop extends Thread implements Threadx {

    private boolean alive;
    private boolean stop;
    
    public ThreadLedStop(){
        alive = true;
        stop  = false;
    }

    public void Pause(){
        stop = true;
        this.interrupt();
    }

    public void UnPause(){
        stop = false;
    }


    public void Kill(){
        stop  = true;
        alive = false;
        this.interrupt();
    }


    @Override
    public void run(){

        while(alive){
            
            if(stop)
                continue;
            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
                Mechanism.ledsOff();
                continue;
            }

            Mechanism.ledOn(2);
            Mechanism.ledOn(1);

            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
                Mechanism.ledsOff();
                continue;
            }
            Mechanism.ledsOff();
        }
    }
}