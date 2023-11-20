
public class CalibrationThreadX extends Thread{
    private final Axis axisX;

    public CalibrationThreadX(Axis axis){
        this.axisX = axis;
    }

    public void initializeCalibrationX(){

         //align X
        axisX.moveForward();
        while (axisX.getPos() == -1) { } //stay in loop, se estiver em movimento
        axisX.stop();
        
        System.out.println("getPosX =" + axisX.getPos());
        if(axisX.getPos() != 1){
            axisX.gotoPos(1);
        }
    }
    

    @Override
    public void run(){
        this.initializeCalibrationX();
    }

}
