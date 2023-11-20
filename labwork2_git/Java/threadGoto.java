
public class threadGoto extends Thread{
    private final Axis axis;
    private final int pos;

    public threadGoto(Axis axis,int pos){
        this.axis = axis;
        this.pos = pos;
    }

    public void initializeGoto(){
       
        axis.gotoPos(pos);
        while(axis.getPos() != pos){}
        axis.stop();
    }
    
    @Override
    public void run(){
        this.initializeGoto();
    }
}