import java.util.LinkedHashMap;
import java.util.Map;
/**
 * this class sorts map by using merge sort algorithm
 * */
public class mergeSort {
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
     * constructor of mergeSort class it initialize originalMap and sortedMap variables
     * @param originalMap original Map variable */
    public mergeSort(myMap originalMap) {
        this.originalMap = originalMap;
        sortedMap = new myMap(originalMap);

        aux = new Character[originalMap.getMapSize()];

        int idx = 0;
        for(Map.Entry<Character,info> entry : originalMap.getMap().entrySet()){
            aux[idx] = entry.getKey();
            idx++;
        }


    }
    /**
     * this method prints map in sorted order
     * */
    public void printSortedMap() {
        System.out.println("\n\nThe Merge sorted map: ");
        for(Map.Entry<Character,info> entry : sortedMap.getMap().entrySet()) {
            Character key = entry.getKey();
            info value = entry.getValue();

            System.out.println("Letter: "+key+" - Count: " + value.getCount()+ " - Words: " + value.getWords());
        }
    }
    /**
     * it sorts map by using merge sort algorithm
     * */
    public void sort(){
        sort(0, originalMap.getMapSize() - 1);
    }
    /**
     * it sorts map by using merge sort algorithm
     * @param l left most index of array
     * @param r right most index of array
     * */
    private void sort(int l, int r)
    {
        if (l < r) {
            int m = l + (r - l) / 2;

            sort(l, m);
            sort(m + 1, r);

            merge(l, m, r);
        }
    }
    /**
     * it sorts map by using merge sort algorithm
     * @param l left most index of array
     * @param m middle index of array
     * @param r right most index of array
     * */
    private void merge( int l, int m, int r)
    {

        Character [] temp = aux.clone();
        int n1 = m - l + 1;
        int n2 = r - m;

        int i = 0, j = 0;

        int k = l;
        while(i < n1 && j < n2){

            if(sortedMap.getMap().get(temp[l + i]).getCount() <= sortedMap.getMap().get(temp[m + 1 + j]).getCount()){
                //sortedMap.getMap().put(aux[l + i], originalMap.getMap().get(aux[l + i]));
                aux[k] = temp[l+i];
                i++;
            }else{
                //sortedMap.getMap().put(aux[m + 1 + j], originalMap.getMap().get(aux[m + 1 + j]));
                aux[k] = temp[m + 1 + j];
                j++;
            }
            k++;
        }

        while(i < n1){
            //sortedMap.getMap().put(aux[l + i], originalMap.getMap().get(aux[l + i]));
            aux[k] = temp[l+i];
            i++;
            k++;
        }

        while(j < n2) {
            //sortedMap.getMap().put(aux[m + 1 + j], originalMap.getMap().get(aux[m + 1 + j]));
            aux[k] = temp[m + 1 + j];
            j++;
            k++;
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
     * returns sorted map
     * @return sorted map
     * */
    public myMap getSortedMap() {
        LinkedHashMap<Character, info> newMap = new LinkedHashMap<>();
        for (char c : aux)
            newMap.put(c, originalMap.getMap().get(c));

        sortedMap.setMap(newMap);

        return sortedMap;
    }
}
