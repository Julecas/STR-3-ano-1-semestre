
import java.time.format.DateTimeFormatter;
import java.util.Scanner;


public class App {
    
    public static ManualCalibration Mc;
    public static Delivery D;
    public static ListItemInStock Li;
    public static SystemInfo S_info;
    public static SearchItem S_item;
    public static ChangeItem Ci;
    public static AddItem Ad;
    public static Mechanism Mec;
    public static Stock stock;
    public static DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd/MM/yyyy HH:mm:ss");
  
    
    //MAIN****************************************************************************************************************************
    public static void main(String[] args) throws Exception {

        System.out.println("Labwork2 from Java");
        Storage.initializeHardwarePorts();
        
        Scanner scan    = new Scanner(System.in);
        
        Ad     = new AddItem();
        Ci     = new ChangeItem();
        S_item = new SearchItem();
        S_info = new SystemInfo();
        Li     = new ListItemInStock();
        D      = new Delivery();
        Mc     = new ManualCalibration();
        stock  = new Stock();
        Mec    = new Mechanism(stock);

        menu(Ad,Mec,stock,formatter,scan);

        Mec.close();
        scan.close();
    }
     //***********************************************************************************************************************************

    public static void menu(AddItem Ad, Mechanism Mec, Stock stock, DateTimeFormatter formatter,Scanner scan ) throws InterruptedException {
       
        String input;
        char c;
        
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
                "   (M)anual calibration  \n"+
                "   (A)uto calibration    \n"+
                "   (S)earch Items        \n"+
                "   Add (I)tem            \n"+
                "   (D)eliver Item        \n"+
                "   (C)hange Item Location\n"+
                "   (P)rint System Info   \n"+
                "   (L)ist all Items      \n"+
                "   (Q)uit                "
            );
            
            System.out.print("Enter an option:");
            
            input = scan.nextLine();

            if( input.isEmpty() ||  Character.toUpperCase( input.charAt(0) ) == 'Q'){
                scan.nextLine();
                return;
            }
            for( int i = 0;i < input.length();++i ){

                c = Character.toUpperCase(input.charAt(i));

                switch ( c ) {
                    case 'M': 
                        Mc.manualCalibration(scan,formatter,Mec,stock); 
                        break; 

                    case 'A': 
                        Mec.autoCalibrate(); 
                        break; 

                    case 'I':  
                        Ad.AddItems(scan,formatter,Mec,stock); 
                        break;

                    case 'C':
                        Ci.ChangeItems(scan,formatter,Mec,stock);
                        break;

                    case 'L':
                        Li.ListItemsInStock(scan,stock,formatter);
                        break;

                    case 'P':
                        S_info.SystemInformation(scan,stock,formatter); 
                        break;

                    case 'S':
                        S_item.SearchItems(scan,formatter,Mec,stock); 
                        break;

                    case 'D':
                        D.Deliver(scan,formatter,Mec,stock);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
   