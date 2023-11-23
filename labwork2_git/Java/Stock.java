import java.time.LocalDate;

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
    public int Add_item(String Name, int ref, LocalDate date,int z,int x){

        if( IsPosValid(z,x) ){
            return -2;
        }
        
        if( IsPosOccupied(z, x) ){
            return -1;
        }

        StockMatrix[ z ][ x ] = new Cell(  Name,  ref,  date );
        return 0;

    }

    private boolean IsPosValid(int z, int x){
        return z < 0 || z > Dimz ||
            x < 0 || x > Dimx;
    }

    private boolean IsPosOccupied( int z, int x){
        return StockMatrix[ z ][ x ] != null;
    }

    private class Cell{
    
        String    Name;
        int       ref;
        LocalDate date;

        public Cell(String Name, int ref, LocalDate date){

            this.Name = new String(Name);
            this.ref  = ref;
            this.date = date;
        }
    }

}

