
public class CalibrationThreadZ extends Thread{
    private final Axis axisZ;

    public CalibrationThreadZ(Axis axis){
        this.axisZ = axis;
    }

    public void initializeCalibrationZ(){
        
        //align Z
        axisZ.moveForward();
        while (axisZ.getPos() == -1) { } //stay in loop, se estiver em movimento
        axisZ.stop();

        System.out.println("getPosZ =" + axisZ.getPos());
        if(axisZ.getPos() != 2){
            axisZ.gotoPos(2);
        }
    }
    

    @Override
    public void run(){
        this.initializeCalibrationZ();
    }

}
