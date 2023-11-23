
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
        //to be developed inJAVA
     
