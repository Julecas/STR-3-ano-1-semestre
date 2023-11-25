import java.time.LocalDateTime;

public class Cell{
    
    String        Name;
    int           ref;
    LocalDateTime date;
    boolean       empty;//true if empty

    public Cell(String Name, int ref, LocalDateTime date){

        this.Name  = new String(Name);
        this.ref   = ref;
        this.date  = date;
        this.empty = false;
    }

    public Cell(){
        
        this.Name  = new String();
        this.ref   = 0;
        this.date  = null;
        this.empty = true;
    }
}
