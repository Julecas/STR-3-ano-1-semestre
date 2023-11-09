public class AxisX implements Axis{
   
    @Override
    public void moveForward(){
        Storage.moveXRight();
    }
    @Override
    public void moveBackward(){
        Storage.moveXLeft();
    }
    @Override
    public void stop(){
        Storage.stopX();
    }
    @Override
    public int getPos(){
        return Storage.getXPos();
    }
     @Override
     public void gotoPos(int pos){
        //TODO auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method");
     }
}