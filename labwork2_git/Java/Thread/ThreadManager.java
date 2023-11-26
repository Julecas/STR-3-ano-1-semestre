import java.util.ArrayList;

public class ThreadManager {
    
    private ArrayList<Thread> Threads;
    
    public ThreadManager(){
        Threads = new ArrayList<Thread>();
    }

    public void AddThread(Thread t){
        Threads.add(t);
    }

    public void ResumeAll(){
        
        for( Thread t: Threads ){
            t.resume();
        }
    }  

    public void StopAll(){
        
        for( Thread t: Threads ){
            t.suspend();
        }
    }   

    public void KillAll(){
        
        for( Thread t: Threads ){
            t.stop();
        }
    }

}
