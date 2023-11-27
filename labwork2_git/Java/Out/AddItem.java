
import java.time.format.DateTimeFormatter;
import java.time.LocalDateTime;
import java.util.Scanner;

public class AddItem {

    public int z,x,ref;
    public int[] pos;
    public String LoadName,input;
    public LocalDateTime ShipDate;


    public AddItem(){
        this.z = 0;
        this.x = 0;
        this.ref = 0;
        this.ShipDate = null;
        this.LoadName = null;
        this.input = null;
        this.pos = new int[2];
    }


    public void AddItems(Scanner scanner, DateTimeFormatter formatter, Mechanism  Mec, Stock stock ){

        //scanner.nextLine();

        GetInput:
        while(true){
        
            //Perguntar coordenadas
            while(true){
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
                    continue;
                }
                
                if( pos[0] == -1){
                    return;
                }

                x = pos[0];
                z = pos[1];
                
                System.out.println("Physical check or only stock check(p/S)?");
                input = scanner.nextLine();
                if( !input.isBlank() && Character.toUpperCase(input.charAt(0)) == 'P' ){   
                    
                    try {

                        if( Mec.checkCell(pos[1], pos[0]) ){
                            System.out.println("Position IS ocuppied!");
                            continue;
                        }

                    } catch (Exception e) {
                        System.out.println("Erro "+e);
                    }
                }
                break;
            }
            //Perguntar data
            while(true){

                System.out.println("Insert Shipment date(DD/MM/YYYY hh:mm:ss) or \'q\' to quit:");
                input = scanner.nextLine();
                input = input.isBlank() ? "n" : input;

                if( Character.toUpperCase(input.charAt(0)) == 'Q' ){
                    return;
                }

                try {
                    ShipDate = LocalDateTime.parse(input,formatter);
                } catch (Exception e) {
                    System.out.println("Invalid input!\n"+ e);
                    continue;
                }
                break;
            }

            //load name
            
            while(true){
                System.out.println("Insert Load Name or \'q\' to quit: ");
                input = scanner.nextLine();
                
                if( input.isBlank()){ continue; }

                if( Character.toUpperCase(input.charAt(0)) == 'Q' ){
                    return;
                }
                LoadName = input;
                break;
            }
            //reference code
            while(true){
                System.out.println("Insert reference code or \'q\' to quit: ");
                input = scanner.nextLine();
                input = input.isBlank() ? "n" : input;

                if( Character.toUpperCase(input.charAt(0)) == 'Q' ){
                    return;
                }

                try{
                    ref = Integer.parseInt(input);
                }catch(Exception e){
                    System.out.println("Invalid input!\n"+ e);
                    continue;
                }
                break;
            }
            System.out.println("Is this correct(Y,n)?\n"+
                               "    Position:      ("+x+","+z+")\n"+
                               "    Reference:     "+ref+"\n"+
                               "    LoadName:      "+LoadName+"\n"+
                               "    Shipment Date: "+ShipDate.format(formatter)); 
        
            input = scanner.nextLine();
            input = input.isBlank() ? "y" : input;

            if( input.indexOf(0) == 'n' || input.indexOf(0) == 'N' ){
                continue;
            }

            int res = stock.Add_item(LoadName, ref, ShipDate, z, x);
        
            switch(res){
                case -2:
                    System.out.println("Coordinates ("+x+","+z+") not valid");
                    continue GetInput;
                case -1:
                    System.out.println("Position ("+x+","+z+") is already Ocupied");
                    continue GetInput;
            }
            break;
        }
        try {
            while(true){

                Mec.GotoReference();
                System.out.println("Insert pallet in Cage and Press any key to continue");
                scanner.nextLine();//wait for enter
                Mec.linguarudo();//para meter a lingua para dentro :P

                if( Mechanism.getPalleteSen() == 1 ){
                    
                    Mec.putPartInCell(z,x);
                    break;
                }
                System.out.println("Missing Pallet!!");
            }
        }catch (InterruptedException e) {
            System.out.println("Error Function Main-->AddItem-->puPartInCellmenu" + e);
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
