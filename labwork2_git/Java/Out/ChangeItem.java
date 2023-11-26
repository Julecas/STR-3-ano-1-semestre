import java.time.format.DateTimeFormatter;
import java.util.Scanner;

public class ChangeItem {

    public String input;
    public int[] pos;
    public int x1,z1,x2,z2;

    public ChangeItem(){
        this.input = null;    
        this.pos   = new int[2];   
        this.x1 = 0;   
        this.z1 = 0; 
        this.x2 = 0; 
        this.z2 = 0; 
    }

    public void ChangeItems(Scanner scanner, DateTimeFormatter formatter, Mechanism  Mec, Stock stock ){

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

            if(pos[0] == -1){
                return;
            }
            
            if( !stock.IsPosOccupied(pos[1], pos[0]) ){
                System.out.println("Position is NOT ocuppied!");
                continue;
                //TODO: Meter opcao de verificar
            }

            x1  = pos[0];
            z1  = pos[1];
            pos = null; 
            break;
        }
        while(true){

            System.out.println("Insert the new position position(x,z) or \'q\' to quit: ");

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

            if(pos[0] == -1){
                return;
            }
            
            if( stock.IsPosOccupied(pos[1], pos[0]) ){
                System.out.println("Position IS ocuppied!");
                continue;
                //TODO: Meter opcao de verificar
            }

            x2  = pos[0];
            z2  = pos[1];
            pos = null; 
            break;
        }
         

        try {
            Mec.takePartFromCell(z1,x1);
        } catch (InterruptedException e) {
            System.out.println("Error Trying to take Item in Main-->ChangeItem-->takePartFromCell");
            return;
        }
        
        try {
            Mec.putPartInCell(z2,x2);
        } catch (InterruptedException e) {
            System.out.println("Error Trying to Put Item in Main-->ChangeItem-->putPartInCell");
            return;
        }

        stock.ChangeItems(z1,x1,z2,x2);

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
