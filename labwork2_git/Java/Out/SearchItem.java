import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Scanner;

public class SearchItem {

    String  input;
    char    caux;
    int[]   pos; 
    int     ref; 
    ArrayList<int[]> cells;

    public SearchItem(){
        this.input = null;
        this.pos   = new int[2];
        this.ref = 0;
        this.cells = new ArrayList<int[]>();
    }

    public void SearchItems(Scanner scanner, DateTimeFormatter formatter, Mechanism  Mec, Stock stock ){

        
        //Perguntar Se ]e ref ou pos
        while(true){
   
            System.out.print("\033[H\033[2J");  
            
            System.out.println(
                "++=========================================================================++      \n" +
                "||   _____                           __       __  ___                      ||      \n" +
                "||  / ___/ ___   ____ _ _____ _____ / /_     /  |/  /___   ____   __  __   ||      \n" +
                "||  \\__ \\ / _ \\ / __ `// ___// ___// __ \\   / /|_/ // _ \\ / __ \\ / / / /   ||\n" +
                "|| ___/ //  __// /_/ // /   / /__ / / / /  / /  / //  __// / / // /_/ /    ||      \n" +
                "||/____/ \\___/ \\__,_//_/    \\___//_/ /_/  /_/  /_/ \\___//_/ /_/ \\__,_/     || \n" +
                "++=========================================================================++      \n"
            );                                                                       

            System.out.println("Search by reference, position or quit(R/p/q)?");
            
            input = scanner.nextLine();
            caux  = input.isBlank() ? 'R' :input.charAt(0);
            
            if(  caux == 'q' ||  caux == 'Q'){
                return;

            }else if(  caux == 'p' ||  caux == 'P'){
                
                System.out.println("Insert the position(x,z) or \'q\' to quit: ");

                input = scanner.nextLine();
                input = input.isBlank() ? "n" : input;

                if( Character.toUpperCase(input.charAt(0)) == 'Q' ){
                    return;
                }
                try {
                    pos = ReadPos(input);
                } catch (Exception e) {
                    System.out.println("Invalid input!\n"+ e);
                    System.out.println( "Press any key to continue");
                    scanner.nextLine();
                    continue;
                }

                if(pos[0] == -1){
                    return;
                }
                int res = stock.GetCellsByPos(pos[1], pos[0]);

                if( res == -2 ){
                    System.out.println("Invalid position("+pos[0]+","+pos[1]+")!\n");
                    System.out.println( "Press any key to continue");
                    scanner.nextLine();
                    continue;
                }
                if( res == -1 ){
                    System.out.println("Position empty("+pos[0]+","+pos[1]+")!\n");
                    System.out.println( "Press any key to continue");
                    scanner.nextLine();
                    continue;
                }

                System.out.println(
                    "The Item's reference at("+pos[0]+","+pos[1]+")\n"+
                    "Is: "+res
                );
                System.out.print("Press any key to continue: ");
                scanner.nextLine();

            }else{
                
                System.out.println("Insert the Reference or \'q\' to quit: ");
                
                input = scanner.nextLine();
                input = input.isBlank() ? "n" : input;

                if( Character.toUpperCase(input.charAt(0)) == 'Q' ){
                    return;
                }
                try {
                    ref = Integer.parseInt(input);
                } catch (Exception e) {
                    System.out.println("Invalid input!\n"+ e);
                    System.out.println( "Press any key to continue");
                    scanner.nextLine();
                    continue;
                }

                cells = stock.GetCellsByRef(ref);

                if( cells.size() == 0 ){
                    System.out.println("There are no Items with the reference number("+ref+")");
                    System.out.print("Press any key to continue: ");
                    scanner.nextLine();
                }else{
                    for(int[] p:cells){
                        System.out.println("("+((int)p[0] + 1) +","+((int)p[1] + 1  )+")");
                    }
                        System.out.println( "Press any key to continue");
                        scanner.nextLine();
                }
            }
        }
    }

    private int[] ReadPos(String input){

        int[] pos = new int[2];
        String aux;

        if( input.charAt(0) == 'q' ){
            pos[0] = -1;
            return pos;
        }

        aux = input.substring(0, input.indexOf(','));

        try{
            pos[0] = Integer.parseInt(aux);
        }catch(Exception e){
            throw e;
        }

        aux = input.substring(input.indexOf(',') + 1);

        try{
            pos[1] = Integer.parseInt(aux);
        }catch(Exception e){
            throw e;
        }
        
        return pos;
    }
    
}

