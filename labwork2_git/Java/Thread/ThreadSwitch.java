import java.util.ArrayList;

public class ThreadSwitch extends Thread {

    private ThreadManager   thread_manager;
    private ThreadLedStop   thread_ledS;
    private byte            PortValue;
    private boolean         paused;
    private Mechanism       mec;
    private Stock           stock;
    
    public ThreadSwitch( ThreadManager t,Mechanism mec,Stock stock){

        this.mec            = mec;
        this.paused         = false;
        this.thread_manager = t;
        this.thread_ledS    = new ThreadLedStop();
        this.PortValue      = 0;
        this.stock          = stock;
    }

    private void Stop(){

        //guarda valores
        paused      = true;
        PortValue   = Storage.ReadPort(2);
        //parar eixos
        Mechanism.axisX.stop();
        Mechanism.axisZ.stop();
        Mechanism.axisY.stop();
        //efetuar interrupção

        thread_manager.StopAll();
        thread_ledS.start();
    }

    private void Resume(){

        Storage.WritePort(2, PortValue);
        thread_manager.ResumeAll();
        Storage.WritePort(2, PortValue);
        paused = false;
        thread_ledS.Kill();
        thread_ledS = null;
        thread_ledS = new ThreadLedStop();
    }
   
    private void Reset(){

        thread_manager.KillAll();
        mec.Reset();
        stock.Reset();
        thread_ledS.Kill();
    } 

    @Override
    public void run(){

        while(true){
            
            if(Mechanism.switch1Pressed() && (!paused)){//remove active shipment date pallete 
                removeActiveShipmentDate();   
                while( Mechanism.switch1Pressed() ){}
                continue;
            }
            if(Mechanism.bothSwithcesPressed() && (!paused)){//interrupt sistema
                Stop();
                while( Mechanism.bothSwithcesPressed() ){}
                continue;
            }
            if(Mechanism.switch1Pressed() && (paused)){//resume sistema
                Resume();
                while( Mechanism.switch1Pressed() ){}
                continue;
            }
            if(Mechanism.switch2Pressed() && (paused)){//reset sistema
                Reset(); 
                while( Mechanism.switch2Pressed() ){}
                continue;
            }
        }
    }


    public void removeActiveShipmentDate(){

        ArrayList<int[]> pos = stock.GetItemsReady();

        for( int[]  p: pos ){
            try {
                mec.takePartFromCell(p[1], p[0]);
            } catch (InterruptedException e) {
                System.out.println( "Error"+e );
            }
            stock.RemoveItem(p[1], p[0]);
            try {
                mec.GotoReference();
                System.out.println("Remove from Cage");
                Thread.sleep(250);
                mec.linguarudo();
            } catch (InterruptedException e) {
            }
        }
    }  
        
}