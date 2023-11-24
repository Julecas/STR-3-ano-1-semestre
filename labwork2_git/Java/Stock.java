import java.time.LocalDateTime;

public class Stock {
    
    private static final int Dimx = 3;
    private static final int Dimz = 3;
    private Cell[][] StockMatrix;
    
    public Stock() {
        StockMatrix = new Cell[Dimz][Dimx];
    }

    //-2 pos errada
    //-1 pos ocupada
    // 0 succefull 
    public int Add_item(String Name, int ref, LocalDateTime date,int z,int x){

        if( IsPosValid(z,x) ){
            return -2;
        }
        
        if( IsPosOccupied(z,x) ){
            return -1;
        }

        StockMatrix[ z ][ x ] = new Cell(  Name,  ref,  date );
        return 0;

    }

    private boolean IsPosValid(int z, int x){
        return
               z < 0 || z > Dimz
            || x < 0 || x > Dimx;
    }

    private boolean IsPosOccupied( int z, int x){
        
        return 
            StockMatrix[ z ][ x ] == null || StockMatrix[ z ][ x ].empty ?
                false : true;    
    }

    private class Cell{
    
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
}

