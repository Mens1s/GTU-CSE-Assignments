import java.util.Map;
/**
 * this class sorts map by using bubble sort algorithm
 * */
public class bubbleSort {
    /**
     * myMap variable that holds original map*/
    private final myMap originalMap;
    /**
     * myMap variable that holds sorted map*/
    private myMap sortedMap;
    /**
     * character list that holds char in the original and sorted map*/
    private final Character [] aux;
    /**
     * constructor of bubbleSort class it initialize originalMap and sortedMap variables
     * @param originalMap original Map variable */
    public bubbleSort(myMap originalMap){
        this.originalMap = originalMap;
        this.sortedMap = new myMap(originalMap);
        this.aux = new Character[originalMap.getMap().size()];
        int i = 0;
        for (Character chr : originalMap.getMap().keySet()) {
            aux[i] = chr;
            i++;
        }
    }
    /**
     * Sort function that provides bubble sort algorithm to sort map*/
    public void sort(){
        int n = aux.length;
        for (int i = 0; i < n-1; i++) {
            for (int j = 0; j < n-i-1; j++){
                if(sortedMap.getMap().get(aux[j]).getCount() > sortedMap.getMap().get(aux[j+1]).getCount()){
                    Character temp = aux[j];
                    aux[j] = aux[j+1];
                    aux[j+1] = temp;
                }
            }
        }
        updateSortedMap();
    }
    /**
     * this method updates sorted map by using aux array
     * */
    private void updateSortedMap(){
        sortedMap = new myMap("");
        for (Character chr : aux) {
            sortedMap.getMap().put(chr, originalMap.getMap().get(chr));
        }
    }
    /**
     * this method prints map in sorted order
     * */
    public void printSortedMap() {
        System.out.println("\n\nThe Bubble sorted map: ");
        for(Map.Entry<Character,info> entry : sortedMap.getMap().entrySet()) {
            Character key = entry.getKey();
            info value = entry.getValue();

            System.out.println("Letter: "+key+" - Count: " + value.getCount()+ " - Words: " + value.getWords());
        }
    }
}
