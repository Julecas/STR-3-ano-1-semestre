


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

        int currentPos = this.getPos();

        if( currentPos == pos )
            return;
        
        else if( currentPos > pos )
            //Means im on left of the goal
            this.moveBackward();
        else
            this.moveForward();
        
        
        while( this.getPos() != pos ){continue;}//prende

        this.stop();
     }
}