import java.time.format.DateTimeFormatter;
import java.util.Scanner;

public class Delivery {

    String input; 
    char caux;
    int[] pos;
    int ref;


    public Delivery(){
        this.pos = new int[2]; 
        this.input = null;
        this.ref = 0;
    }

     public void Deliver(Scanner scanner, DateTimeFormatter formatter, Mechanism  Mec, Stock stock ){

        while(true){

            System.out.print("\033[H\033[2J");  
            System.out.println(
                "    ____         __ _                       __  ___                         \n" +
                "   / __ \\ ___   / /(_)_   __ ___   _____   /  |/  /___   ____   __  __     \n" +
                "  / / / // _ \\ / // /| | / // _ \\ / ___/  / /|_/ // _ \\ / __ \\ / / / /  \n" +
                " / /_/ //  __// // / | |/ //  __// /     / /  / //  __// / / // /_/ /       \n" +
                "/_____/ \\___//_//_/  |___/ \\___//_/     /_/  /_/ \\___//_/ /_/ \\__,_/    \n"
            );

            System.out.print("Choose the Item by (r)eference, (p)osition or (Q)uit: ");

            input = scanner.nextLine();

            if( input.isBlank() ){ return; }

            caux =  Character.toUpperCase( input.charAt(0));

            if( caux == 'Q' ){
                return;
            }
            if( caux == 'P'){
                
                while( true ){
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
                    
                    System.out.println("Physical check or only stock check(p/S)?");
                    input = scanner.nextLine();
                    if( !input.isBlank() && Character.toUpperCase(input.charAt(0)) == 'P' ){   
                        try {

                            if( !Mec.checkCell(pos[1], pos[0]) ){
                                System.out.println("Position is NOT ocuppied!");
                                continue;
                            }
                        }catch (Exception e) {
                            System.out.println("Erro "+e);
                        }
                        if( stock.IsPosOccupied(pos[1], pos[0]) ){
                            stock.RemoveItem(pos[1], pos[0]);
                        }

                    }else{
                        if( !stock.IsPosOccupied(pos[1], pos[0]) ){
                                System.out.println("Position is NOT ocuppied!");
                                continue;
                        }

                        stock.RemoveItem(pos[1], pos[0]);
                    }

                    try {
                        Mec.takePartFromCell(pos[1], pos[0]);
                        Mec.GotoReference();
                        System.out.println("Remove from Cage and Press any key to continue");
                        scanner.nextLine();//wait for enter
                        Mec.linguarudo();//para meter a lingua para dentro :P
                    } catch (InterruptedException e) {
                        //TODO: Erro
                    }
                    break;
                }
            }else if( caux == 'R' ){

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

                pos = stock.GetPosByRef(ref);
                if( pos[0] == -1 ){
                    System.out.println( "Position is empty!" );
                    System.out.println( "Press any key to continue");
                    scanner.nextLine();
                    continue;
                }


                stock.RemoveItem(pos[1], pos[0]);
                
                try {
                    Mec.takePartFromCell(pos[1], pos[0]);
                    Mec.GotoReference();
                    System.out.println("Remove from Cage and Press any key to continue");
                    scanner.nextLine();//wait for enter
                    Mec.linguarudo();//para meter a lingua para dentro :P
                } catch (InterruptedException e) {
                    //TODO: Erro
                }
                break;
            }
            return;
        }
    }

    //TODO: QUE NOJO
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
