import java.util.ArrayList;

public class ThreadManager {
    
    public ArrayList<Threadx> Threads;
    
    public ThreadManager(){
        Threads = new ArrayList<Threadx>();
    }

    public void AddThread(Threadx t){
        Threads.add(t);
    }

    public void ResumeAll(){
        
        for( Threadx t: Threads ){
            t.UnPause();
        }
    }  

    public void StopAll(){
        
        for( Threadx t: Threads ){
            t.Pause();
        }
    }

    public void KillAll(){
        
        for( Threadx t: Threads ){
            t.Kill();
        }
    }

}
