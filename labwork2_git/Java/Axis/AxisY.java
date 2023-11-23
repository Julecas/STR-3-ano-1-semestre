public class AxisY implements Axis{
   
    @Override
    public void moveForward(){
        Storage.moveYInside();
    }
    @Override
    public void moveBackward(){
        Storage.moveYOutside();
    }
    @Override
    public void stop(){
        Storage.stopY();
    }
    @Override
    public int getPos(){
        return Storage.getYPos();
    }
     @Override
     public void gotoPos(int pos){
        //TODO auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method");

        //to be developed inJAVA
     }
}