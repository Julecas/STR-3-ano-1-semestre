public class AxisZ implements Axis{
   
    @Override
    public void moveForward(){
        Storage.moveZUp();
    }
    @Override
    public void moveBackward(){
        Storage.moveZDown();
    }
    @Override
    public void stop(){
        Storage.stopZ();
    }
    @Override
    public int getPos(){
        return Storage.getZPos();
    }
     @Override
     public void gotoPos(int pos){
        //TODO auto-generated method stub
        //throw new UnsupportedOperationException("Unimplemented method");

        //ASSUMING ITS NOT ALREADY MOVING
        if(Storage.getZPos() == pos){//if in right position
            return;
        }
        
        if(Storage.getZPos() > pos){
            //System.out.println("DEBUGG");
            Storage.moveZDown();
            while(Storage.getZPos() != pos){ };
            Storage.stopZ();
        }

        if(Storage.getZPos() < pos){
            Storage.moveZUp();
            while(Storage.getZPos() != pos){ };
            Storage.stopZ();
        }

        //to be developed inJAVA
     }
}