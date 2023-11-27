import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Scanner;

public class SystemInfo {

    public void SystemInformation(Scanner scanner,Stock stock, DateTimeFormatter formatter){
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

    
}
