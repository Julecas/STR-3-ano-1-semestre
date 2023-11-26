import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Scanner;

public class ListItemInStock{

    public void ListItemsInStock(Scanner scanner,Stock stock, DateTimeFormatter formatter){
        
        System.out.print("\033[H\033[2J");

        ArrayList<Cell> Items = stock.GetItems();
        
        if( Items.size() == 0 ){
            System.out.println("Storage is empty!");
            System.out.println( "Press any key to exit");
            scanner.nextLine();
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
}