import java.time.LocalDateTime;

public class Cell{
    
    String          Name;
    int             ref;
    LocalDateTime   date;
    boolean         empty;//true if empty
    ThreadShipDate  thread;

    public Cell(String Name, int ref, LocalDateTime date){

        this.Name   = new String(Name);
        this.ref    = ref;
        this.date   = date;
        this.empty  = false;
        this.thread = new ThreadShipDate(this); 
        this.thread.start();
    }

    public Cell(){
        
        this.Name   = new String();
        this.ref    = 0;
        this.date   = null;
        this.empty  = true;
        this.thread = null;
    }
}