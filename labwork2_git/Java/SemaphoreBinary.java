import java.util.concurrent.Semaphore;

public class SemaphoreBinary {
    
    private Semaphore sem;
    private boolean   Aquired;
    private int       counter;

    public SemaphoreBinary(){
        sem     = new Semaphore(1);
        Aquired = false;
        counter = 0;
    }

    public void ForceAquire(){
        
        ++counter;

        if( !this.Aquired ){
            
            this.Aquired = true;
            try {
                sem.acquire();
            } catch (Exception e) {
                System.out.println("Erro SemaphoreBinary"+e);
            }
        }
    }

    public void Aquire(){
        try {
            sem.acquire();
        } catch (InterruptedException e) {
            System.out.println("Erro SemaphoreBinary"+e);
        }
    }

    public void Release(){

        if( --counter == 0 ){
            sem.release();
            Aquired = false;
        }
    }

    public void ForceRelease(){
        sem.release();
        Aquired = false;
    }

    public boolean IsSemAquired(){
        return Aquired;
    }
}
