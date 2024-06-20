import java.util.LinkedHashMap;
import java.util.Map;

public class mergeSort {
    private myMap originalMap;
    private myMap sortedMap;
    Character [] aux;

    public mergeSort(myMap originalMap) {
        this.originalMap = originalMap;
        sortedMap = new myMap(originalMap);

        aux = new Character[originalMap.getMapSize()];

        int idx = 0;
        for(Map.Entry<Character,info> entry : originalMap.getMap().entrySet()){
            aux[idx] = entry.getKey();
            idx++;
        }

        sort(0, originalMap.getMapSize() - 1);
    }

    void sort(int l, int r)
    {
        if (l < r) {
            int m = l + (r - l) / 2;

            sort(l, m);
            sort(m + 1, r);

            merge(l, m, r);
        }
    }

    void merge( int l, int m, int r)
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
    private void updateSortedMap(){
        for (Character chr : aux) {
            sortedMap.getMap().put(chr, originalMap.getMap().get(chr));
        }
    }
    public myMap getSortedMap() {
        LinkedHashMap<Character, info> newMap = new LinkedHashMap<>();
        for (char c : aux)
            newMap.put(c, originalMap.getMap().get(c));

        sortedMap.setMap(newMap);

        return sortedMap;
    }
}
