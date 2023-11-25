
public class Mechanism {
    
    public void ledOn(int ledNumber){
        Storage.ledOn(ledNumber);
    }

    public void ledsOff(){
        Storage.ledsOff();
    }

    public boolean switch1Pressed(){
        //todo for now returns false
        if(Storage.getSwitch1() == 1){
            return true;
        }
        return false;
    }

    public boolean switch2Pressed(){
        //todo for now returns false
        return false;
    }

    public boolean bothSwithcesPressed(){
        //todo for now returns false
        return false;
    }

    public void putPartInCell( ThreadGoto X,ThreadGoto Y,ThreadGoto Z ){
        //todo
    }

    public void takePartFromCell(){
        //todo
    }
}
