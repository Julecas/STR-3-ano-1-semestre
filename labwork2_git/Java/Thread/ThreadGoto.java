import java.util.concurrent.ArrayBlockingQueue;

public class ThreadGoto extends Thread{

    private ArrayBlockingQueue<Integer> mbx_pos;
    private static final int MCap = 10;
    private final Axis axis;
    private boolean stop;

    public ThreadGoto(Axis axis){ //constructor
        this.axis = axis;
        this.mbx_pos = new ArrayBlockingQueue<Integer>(MCap);
        stop = false;
    }

    public void kill(){
        stop = true;
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

        if(axis.getPos() == pos){}// se já estiver na posição correta não faz nada
        else{
            axis.gotoPos(pos);
            while(axis.getPos() != pos){}
            axis.stop();
        }

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

        while(!stop){//to keep running
            this.initializeGoto();
        }
    }

    public void AddQueue(int pos){
        this.mbx_pos.add(pos);
    }

}