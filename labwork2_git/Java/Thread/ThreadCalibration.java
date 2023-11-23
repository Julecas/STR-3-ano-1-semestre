

public class ThreadCalibration extends Thread{
    private final Axis axis;

    public ThreadCalibration(Axis axis){
        this.axis = axis;
    }

    public void initializeCalibration(){

        //align X
        axis.moveForward();
        while (axis.getPos() == -1) { } //stay in loop, se estiver em movimento
        axis.stop();
        
        if(axis.getPos() != 1){
            axis.gotoPos(1);
        }
    }
    

    @Override
    public void run(){
        this.initializeCalibration();
    }

}
