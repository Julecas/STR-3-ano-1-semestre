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
        //throw new UnsupportedOperationException("Unimplemented method");
        
        if(Storage.getXPos() == pos){//if in right position
            return;
        }
        
        if(Storage.getXPos() > pos){
            Storage.moveXLeft();
            while(Storage.getXPos() != pos){};
            Storage.stopX();
        }

        if(Storage.getXPos() < pos){
            Storage.moveXRight();
            while(Storage.getXPos() != pos){};
            Storage.stopX();
        }
    }
}       
        //to be developed inJAVA
     
