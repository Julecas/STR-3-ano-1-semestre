
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

        int currentPos = this.getPos();

        if( currentPos == pos )
            return;
        
        else if( currentPos > pos )
            //Means im on left of the goal
            this.moveBackward();
        else
            this.moveForward();
        
        
        while( this.getPos() != pos ){continue;}

        this.stop();
     }
}