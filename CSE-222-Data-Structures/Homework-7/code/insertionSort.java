import java.util.Map;
/**
 * this class sorts map by using insertion sort algorithm
 * */
public class insertionSort {
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
     * constructor of insertion sort class it initialize originalMap and sortedMap variables
     * @param originalMap original Map variable */

    public insertionSort(myMap originalMap) {
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
        for (int i = 1; i < n; ++i) {
            Character key = aux[i];
            int j = i - 1;

            while (j >= 0 && sortedMap.getMap().get(aux[j]).getCount() > sortedMap.getMap().get(key).getCount()) {
                aux[j + 1] = aux[j];
                j = j - 1;
            }
            aux[j + 1] = key;
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
        System.out.println("\n\nThe Insertion sorted map: ");
        for(Map.Entry<Character,info> entry : sortedMap.getMap().entrySet()) {
            Character key = entry.getKey();
            info value = entry.getValue();

            System.out.println("Letter: "+key+" - Count: " + value.getCount()+ " - Words: " + value.getWords());
        }
    }
}
