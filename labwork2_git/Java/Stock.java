import java.time.LocalDateTime;
import java.util.ArrayList;

public class Stock {
    
    private static final int Dimx = 3;
    private static final int Dimz = 3;
    private Cell[][] StockMatrix;
    private int cap;
    private int CapUtilized;

    public Stock() {
        StockMatrix = new Cell[Dimz][Dimx];
        cap         = Dimz*Dimx;
        CapUtilized = 0;
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
        ++CapUtilized;
        return 0;

    }

    public boolean IsPosValid(int z, int x){
        return
               z < 0 || z > Dimz
            || x < 0 || x > Dimx;
    }

    public boolean IsPosOccupied( int z, int x){
        
        return 
            StockMatrix[ z ][ x ] == null || StockMatrix[ z ][ x ].empty ?
                false : true;    
    }

    public ArrayList<int[]> GetCellsByRef(int ref){
        
        ArrayList<int[]> ret = new ArrayList<int[]>();
        int[] i;

        for(int z = 0; z < Dimz;++z){

            for(int x = 0; x < Dimx;++x){

                if( this.StockMatrix[z][x].ref == ref ){
                    i = new int[2];                   
                    i[0] = x;
                    i[1] = z; 
                    ret.add( i );
                }
            }
        }
        return ret;
    }

    public ArrayList<Cell> GetItems(){

        ArrayList<Cell> c = new ArrayList<Cell>();

        for(int z = 0; z < Dimz;++z){

            for(int x = 0; x < Dimx;++x){
                
                if( !this.StockMatrix[z][x].empty ){
                    c.add(StockMatrix[z][x]);
                }
            }
        }
        return c;
    }

    public int GetTotalCap(){ return cap; }

    public int GetCapUsed(){ return CapUtilized;}

}

