import java.util.concurrent.atomic.AtomicBoolean;

public class AxisZ implements Axis{
    
    private AtomicBoolean stop;
    private AtomicBoolean alive;
    
    public AxisZ(){
        stop  = new AtomicBoolean(false);
        alive = new AtomicBoolean(true);
    }

    @Override
    public void Kill(){
        stop.set(true);
        alive.set(false);
    }


    @Override
    public void Pause(){
        stop.set(true);
    }
    @Override
    public void UnPause(){
        stop.set(false);
        
    }


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

        int currentPos = this.getPos();

        if( currentPos == pos )
            return;

        if( currentPos == -1 ){
            
            this.moveForward();
            while( this.getPos() == -1 ){
                while( stop.get() ){
                    if( !alive.get() ){
                        return;
                    }
                }
            }
            this.stop();
        }
        
        if( currentPos > pos )
            //Means im on left of the goal
            this.moveBackward();
        else
            this.moveForward();
        
        
        while( this.getPos() != pos ){
            while( stop.get() ){
                if( !alive.get() ){
                    return;
                }
            }
        }while( this.getPos() != pos && !stop.get()){continue;}

        
        this.stop();
     }
}