import java.util.concurrent.ArrayBlockingQueue;

public class ThreadGoto extends Thread implements Threadx{

    private ArrayBlockingQueue<Integer> mbx_pos;
    private static final int MCap = 10;
    private final Axis axis;
    private boolean stop;
    private boolean alive;

    public ThreadGoto(Axis axis){ //constructor

        Mechanism.thread_manager.AddThread(this);
        this.axis = axis;
        this.mbx_pos = new ArrayBlockingQueue<Integer>(MCap);
        stop  = false;
        alive = true;
    }

    public void Pause(){
        stop = true;
        axis.Pause();
    }

    public void UnPause(){
        stop = false;
        axis.UnPause();
    }


    public void Kill(){
        axis.Kill();
        stop  = true;
        alive = false;
    }

    public int axisCurrentPos(){ 
        return axis.getPos();
    }

    public void initializeGoto(){

        int pos;

        try {
            pos = mbx_pos.take();
        } catch (InterruptedException e) {
            System.out.println("Error: "+ e);
            return;
        }

        //System.out.println("Af,mbx Pos =" + pos);

        axis.gotoPos(pos);
        while(axis.getPos() != pos){}
        axis.stop();

        //release semaphores
        if(this.axis == Mechanism.axisX){
            Mechanism.releaseSemX();
        }
        if(this.axis == Mechanism.axisZ){
            Mechanism.releaseSemZ();
        }
        if(this.axis == Mechanism.axisY){
            Mechanism.releaseSemY();
        }

    }
    
    @Override
    public void run(){

        while(alive){//to keep running
            
            if(!stop)
                this.initializeGoto();
        }
    }

    public void AddQueue(int pos){
        this.mbx_pos.add(pos);
    }

}