import java.util.concurrent.atomic.AtomicBoolean;

public interface Axis {
    
    public void moveForward();
    public void moveBackward();
    public void stop();
    public int getPos();
    public void Kill();
    public void Pause();
    public void UnPause();

    //to implement in JAVA  
    public void gotoPos(int pos);
}
