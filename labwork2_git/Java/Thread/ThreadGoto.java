import java.util.concurrent.ArrayBlockingQueue;

public class ThreadGoto extends Thread{

    private  ArrayBlockingQueue<Integer> mbx_pos;
    private static final int MCap = 10;
    private final Axis axis;

    public ThreadGoto(Axis axis){ //constructor
        this.axis = axis;
        this.mbx_pos = new ArrayBlockingQueue<Integer>(MCap);
    }

    public int axisCurrentPos(){ 
        return axis.getPos();
    }

    public void initializeGoto(){

        int pos;

        try {
            pos = mbx_pos.take();
        } catch (InterruptedException e) {
            System.out.println("Erro: "+ e);
            return;
        }

        System.out.println("Af,mbx Pos =" + pos);

        axis.gotoPos(pos);
        while(axis.getPos() != pos){}
        axis.stop();
    }
    
    @Override
    public void run(){

        while(true){//to keep running
            this.initializeGoto();
        }
    }

    public void AddQueue(int pos){
        this.mbx_pos.add(pos);
    }

}