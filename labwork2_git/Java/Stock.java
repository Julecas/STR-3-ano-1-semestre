import java.time.Duration;
import java.time.LocalDateTime;
import java.util.ArrayList;

public class Stock {
    
    private static final int Dimx = 3;
    private static final int Dimz = 3;
    public  static final int cap = Dimz*Dimx;
    private Cell[][] StockMatrix;
    private int CapUtilized;

    public Stock() {
        StockMatrix = new Cell[Dimz][Dimx];
        CapUtilized = 0;
    }

    //-2 pos errada
    //-1 pos ocupada
    // 0 succefull 
    public int Add_item(String Name, int ref, LocalDateTime date,int z,int x){

        --z;
        --x;
        if( !IsPosValid(z+1,x+1) ){
            return -2;
        }
        
        if( IsPosOccupied(z+1,x+1) ){
            return -1;
        }

        StockMatrix[ z ][ x ] = new Cell(  Name,  ref,  date );
        ++CapUtilized;
        return 0;

    }

    public int RemoveItem(int z,int x){
 
        --z;    
        --x;       
        if( !IsPosOccupied(z+1,x+1) ){
            return -1;
        }
        if( !IsPosValid(z+1, x+1) ){
            return -2;
        }

        StockMatrix[z][x].thread.kill();
        StockMatrix[z][x] = null;
        --CapUtilized;
        return 0;
    }

    //Returns Arraylist of int[2]
    //Where pos 0 --> x
    //And   pos 1 --> z
    public ArrayList<int[]> GetItemsReady(){

        ArrayList<int[]> items = new ArrayList<int[]>();
        int[] iaux;
        LocalDateTime now = LocalDateTime.now();
        LocalDateTime timeaux;

        for(int z = 0; z < Dimz; ++z){

            for(int x =0;x < Dimx; ++x){
                
                timeaux = StockMatrix[ z ][ x ].date;
                
                if( Duration.between( timeaux, now ).toSeconds() <= 0 ){
                    iaux = new int[2];
                    iaux[0] = x;
                    iaux[1] = z; 
                    items.add( iaux );
                }
            }
        }

        return items;

    }

    //Cell at z2,x2 must be empty
    public void ChangeItems(int z1,int x1,int z2,int x2){

        --z1;
        --x1;
        --z2;
        --x2;
        StockMatrix[z2][x2] = StockMatrix[z1][x1];
        StockMatrix[z1][x1] = null;
    }

    public boolean IsPosValid(int z, int x){
        --z;
        --x;
        return
               z < 0 || z < Dimz
            || x < 0 || x < Dimx;
    }

    public boolean IsPosOccupied( int z, int x){
        --z;
        --x;
        return 
            StockMatrix[ z ][ x ] == null || StockMatrix[ z ][ x ].empty ?
                false : true;    
    }


    //Return -1 if empty
    //Return -2 if Number invalid
    public int GetCellsByPos(int z,int x){

        if( !IsPosOccupied(z, x) ){
            return -1;
        }
        if( !IsPosValid(z, x) ){
            return -2;
        }

        return StockMatrix[--z][--x].ref;
    }

    public int[] GetPosByRef(int ref){
        int[] pos = new int[2];
        pos[0] = -1;

        for(int z = 0; z < Dimz;++z){

            for(int x = 0; x < Dimx;++x){

                if( this.StockMatrix[z][x] != null && this.StockMatrix[z][x].ref == ref ){
                    pos[0] = x + 1;
                    pos[1] = z + 1;
                    return pos;
                }
            }
        }
        return pos;
    }

    //Returns ArrayList of int[2]
    //Where pos 0 --> x
    //And   pos 1 --> z
    public ArrayList<int[]> GetCellsByRef(int ref){
        
        ArrayList<int[]> ret = new ArrayList<int[]>();
        int[] i;

        for(int z = 0; z < Dimz;++z){

            for(int x = 0; x < Dimx;++x){

                if( this.StockMatrix[z][x] != null && this.StockMatrix[z][x].ref == ref ){
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
                
                if( StockMatrix[z][x] != null && !this.StockMatrix[z][x].empty ){
                    c.add(StockMatrix[z][x]);
                }
            }
        }
        return c;
    }

    public int GetTotalCap(){ return cap; }

    public int GetCapUsed(){ return CapUtilized;}

}