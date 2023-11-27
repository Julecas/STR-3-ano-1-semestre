import java.time.format.DateTimeFormatter;
import java.util.Scanner;

public class ManualCalibration {

    int posX,posZ;
    String input;  
    int[] pos;

    public ManualCalibration(){
        
        this.posX = 0;
        this.posZ = 0;
        this.input = null;
        this.pos = new int[2];
    }

    public void manualCalibration(Scanner scanner, DateTimeFormatter formatter, Mechanism  Mec, Stock stock)  throws InterruptedException{
       
        char caux;
        while(true){

            System.out.println("Insert the position(x,z) or \'q\' to quit: ");

            input = scanner.nextLine();
            input = input.isBlank() ? "n" : input;
            caux  = Character.toUpperCase(input.charAt(0));
            if( caux == 'Q' ){
                return;
            }
            try {
                pos = ReadPos(input);
            } catch (Exception e) {
                System.out.println("Invalid input!\n"+ e);
                continue;
            }
            
            if( pos[0] == -1){
                System.out.println("Invalid input!\n");
                continue;
            }
            posX  = pos[0];
            posZ  = pos[1];
            pos = null; 
            Mec.manualCalibrate(posZ,posX);
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