import java.util.Map;
/**
 * this class sorts map by using quick sort algorithm
 * */
public class quickSort {
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
     * constructor of quick sort class it initialize originalMap and sortedMap variables
     * @param originalMap original Map variable */

    public quickSort(myMap originalMap){
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
        quickSortAlgo(aux, 0, n-1);
        updateSortedMap();
    }
    /**
     * runs recursively himself to call partition class */
    private void quickSortAlgo(Character [] arr, int low, int high){
        if (low < high){
            int pi = doPart(arr, low, high);
            quickSortAlgo(arr, low, pi-1);
            quickSortAlgo(arr, pi+1, high);
        }
    }
    /**
     * this part recursively sort mymap class */
    private int doPart(Character [] arr, int l, int h){
        Character pivot = arr[h];

        int i = ( l - 1 );
        for (int j = l; j < h; j++){
            if (sortedMap.getMap().get(arr[j]).getCount() < sortedMap.getMap().get(pivot).getCount()){
                i++;
                Character temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        Character temp = arr[i+1];
        arr[i+1] = arr[h];
        arr[h] = temp;

        return i+1;
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
        System.out.println("\n\nThe Quick sorted map: ");
        for(Map.Entry<Character,info> entry : sortedMap.getMap().entrySet()) {
            Character key = entry.getKey();
            info value = entry.getValue();

            System.out.println("Letter: "+key+" - Count: " + value.getCount()+ " - Words: " + value.getWords());
        }
    }
}
