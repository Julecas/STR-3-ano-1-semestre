import java.util.Scanner;

public class App {
    public static void main(String[] args) throws Exception {
        System.out.println("Labwork2 from Java");

        Storage.initializeHardwarePorts();

        AxisX axisX = new AxisX();
        int op = -1;
        Scanner scan = new Scanner(System.in);
        while(op != 0){
            System.out.println("Enter an option");
            op = scan.nextInt();
            switch(op){
                case 1: axisX.moveForward();   break;
                case 2: axisX.moveBackward();  break;
                case 3: axisX.stop();          break;
                default: break;
            }
        }
        scan.close();
    }
}
