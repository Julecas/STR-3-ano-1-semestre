import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Scanner;
import java.lang.Math;
//import java.util.concurrent.ArrayBlockingQueue;
//import java.util.concurrent.Semaphore;

public class App {
    private static Mechanism Mec;
    private static int posX = 0;
    private static int posZ = 0;
    private static int op = -1;
    private static Stock stock;
    private static DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd/MM/yyyy HH:mm:ss");
    
    //MAIN****************************************************************************************************************************
    public static void main(String[] args) throws Exception {

        posX = 0;
        posZ = 0;
        op = -1;
        stock = new Stock();

        System.out.println("Labwork2 from Java");
        Storage.initializeHardwarePorts();
        Mec = new Mechanism();


        menu();
    }
     //***********************************************************************************************************************************

    public static void menu() throws InterruptedException {
       
        String input;
        Scanner scan = new Scanner(System.in);
        
        while(true){
            System.out.print("\033[H\033[2J");  
            System.out.println(
                    "++================================++  \n" +
                    "||    __  ___                     ||  \n" +
                    "||   /  |/  /___   ____   __  __  ||  \n" +
                    "||  / /|_/ // _ \\ / __ \\ / / / /  ||\n" +
                    "|| / /  / //  __// / / // /_/ /   ||  \n" +
                    "||/_/  /_/ \\___//_/ /_/ \\__,_/    ||\n" +
                    "++================================++\n"
                ); 
              
            System.out.println(
                "Options:\n"+
                "   Auto calibration(a)\n"+
                "   Search Items(s)\n"+
                "   Add Item(i)\n"+
                "   Change Item Location(c)\n"+
                "   Print System Info(p)\n"+
                "   List all Items(l)"
            );
            System.out.print("Enter an option:");
            
            input = scan.nextLine();

            if(input.charAt(0) == 'q'){
                break;
            }

            switch (input.charAt(0)) {
                case 'a': {Mec.autoCalibrate(); break;} //AUTO CALIBRATE DONNE
                case 'i':   {//put part in cell DONE
                    AddItem(scan);
                    break;}
                case 'c':{//to change
                    op = scan.nextInt();
                    posZ = firstDigit(op);
                    posX = lastDigit(op);
                    Mec.takePartFromCell(posZ,posX);
                    break;}
                case 'l':{ListItemsInStock(scan); break;} //LIST ITEMS DONNE
                case 'p':{SystemInfo(scan); break;}//SYS INFO DONNE
                case 's':{SearchItems(scan); break;}//SYS INFO DONNE
                default:{System.out.println( Mec.getPalleteSen());scan.nextLine();break;} //not working
            }
        }
        scan.close();
    }

    public static int firstDigit(int op){
        // To find the last digit of the number : taking modulo with 10
        return  Math.abs(op % 10);
    }
     public static int lastDigit(int op){
        // To find the last digit of the number
        int tm = 0; 
        while (op != 0) {
            tm = Math.abs(op % 10);
            op /= 10;
        }
        return tm;
    }
   
    private static void SearchItems(Scanner scanner){
        
        String  input;
        char    caux;
        int[]   pos   = new int[2];
        int     ref; 
        ArrayList<int[]> cells = new ArrayList<int[]>();
        
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
            caux  = input.charAt(0);
            
            if(  caux == 'q' ||  caux == 'Q'){
                return;

            }else if(  caux == 'p' ||  caux == 'P'){
                
                System.out.println("Insert the position(x,z) or \'q\' to quit: ");

                input = scanner.nextLine();

                try {
                    pos = ReadPos(input);
                } catch (Exception e) {
                    System.out.println("Invalid input!\n"+ e);
                    continue;
                }

                if(pos[0] == -1){
                    return;
                }
                int res = stock.GetCellsByPos(pos[1], pos[0]);

                if( res == -2 ){
                    System.out.println("Invalid position!\n");
                    continue;
                }
                if( res == -1 ){
                    System.out.println("Position empty!\n");
                    continue;
                }

                System.out.println(
                    "The Item's reference at("+pos[0]+","+pos[1]+")\n"+
                    "Is: "+res
                );

            }else{
                
                System.out.println("Insert the Reference or \'q\' to quit: ");
                
                input = scanner.nextLine();
                
                try {
                    ref = Integer.parseInt(input);
                } catch (Exception e) {
                    System.out.println("Invalid input!\n"+ e);
                    continue;
                }

                cells = stock.GetCellsByRef(ref);

                if( cells.size() == 0 ){
                    System.out.println("There are no Items with the reference number("+ref+")");
                }else{
                    for(int[] p:cells){
                        System.out.println("("+p[0]+","+p[1]+")");
                        System.out.println( "Press any key to continue");
                        scanner.nextLine();
                    }
                }
            }
        }
    }

    private static void ListItemsInStock(Scanner scanner){
        
        System.out.print("\033[H\033[2J");

        ArrayList<Cell> Items = stock.GetItems();
        
        if( Items.size() == 0 ){
            System.out.println("Storage is empty!");
            return;
        }

        for(Cell c: Items){
            System.out.println(
                "Item LoadName:      "+c.Name+"\n"+
                "Item reference:     "+c.ref+"\n"+
                "Item Shipment Date: "+c.date.format(formatter)+"\n"+
                "----------------------------");
        }
        System.out.println( "Press any key to exit");
        scanner.nextLine();
    }

    private static void SystemInfo(Scanner scanner){
        System.out.println(
                "\033[H\033[2J"+
                "System Info:\n"+
                "   Total Capacity:"+ stock.GetTotalCap()+"\n"+
                "   Capacity used: "+ stock.GetCapUsed()+ "\n"+
                "   Current Date:  "+LocalDateTime.now().format(formatter)+
                "\n Press any key to exit" 
            );
        scanner.nextLine();
    }


    private static void ChangeItem(Scanner scanner){

        String input;
        int[] pos = new int[2];
        int x1,z1,x2,z2;

        while(true){

            System.out.println("Insert the position(x,z) or \'q\' to quit: ");

            input = scanner.nextLine();

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
         
        //TODO: trocar 

        stock.ChangeItems(z1,x1,z2,x2);

    }

    private static void AddItem(Scanner scanner){

        int z,x,ref;
        int[] pos = new int[2];
        String LoadName,input;
        LocalDateTime ShipDate;

        //scanner.nextLine();

        GetInput:
        while(true){
        
            //Perguntar coordenadas
            while(true){
                System.out.println("Insert the position(x,z) or \'q\' to quit: ");

                input = scanner.nextLine();

                try {
                    pos = ReadPos(input);
                } catch (Exception e) {
                    System.out.println("Invalid input!\n"+ e);
                    continue;
                }
                
                if( pos[0] == -1){
                    return;
                }

                x   = pos[0];
                z   = pos[1];
                pos = null; 
                break;
            }
            //Perguntar data
            while(true){

                System.out.println("Insert Shipment date(DD/MM/YYYY hh:mm:ss) or \'q\' to quit:");
                input = scanner.nextLine();

                try {
                    ShipDate = LocalDateTime.parse(input,formatter);
                } catch (Exception e) {
                    System.out.println("Invalid input!\n"+ e);
                    continue;
                }
                break;
            }

            //load name
            System.out.println("Insert Load Name or \'q\' to quit: ");
            LoadName = scanner.nextLine();

            //reference code
            while(true){
                System.out.println("Insert reference code or \'q\' to quit: ");
                input = scanner.nextLine();

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
            Mec.putPartInCell(z,x);
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            System.out.println("Error put part in cell menu" + e);
        }
    }

    private static int[] ReadPos(String input){

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