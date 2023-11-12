import java.util.LinkedHashMap;
import java.util.Map;

public class myMap {
    private LinkedHashMap<Character, info> map;
    private int mapSize;
    private String str;

    public myMap(myMap obj){
        this.str = obj.getStr();
        this.map = obj.getMap();
        this.mapSize = obj.getMapSize();
    }
    public myMap(String str) {
        this.str = str;
        this.map = new LinkedHashMap<>();
        this.mapSize = 0;
    }
    public void doOperations(){
        System.out.println("Original String: \t\t\t"+str);
        preProcesses();
        System.out.println("Preprocessed String: \t\t"+str);
        if(str.length() == 0) {
            System.out.println("The string is empty!");
            System.out.println("The map is empty!");
            System.out.println("The sorted map is empty!");
            System.out.println("The program is terminated!");
            return;
        }
        toMap();
        printOriginalMap();

        mergeSort sort = new mergeSort(this);
        printSortedMap(sort.getSortedMap());

    }

    private void printSortedMap(myMap sortedMap) {
        System.out.println("\n\nThe sorted map: ");
        for(Map.Entry<Character,info> entry : sortedMap.getMap().entrySet()) {
            Character key = entry.getKey();
            info value = entry.getValue();

            System.out.println("Letter: "+key+" - Count: " + value.getCount()+ " - Words: " + value.getWords());
        }
    }

    private void preProcesses(){
        str = str.toLowerCase().replaceAll("[^a-z\\s]", "");
        boolean flag = false;

        for(char c : str.toCharArray())
            if(Character.isLetter(c))
                flag = true;

        if(!flag) str = "";
        /*
          StringBuilder temp = new StringBuilder();
          str = str.toLowerCase();
          for(int idx = 0; idx < str.length(); idx++)
              if(str.charAt(idx) == ' ' || Character.isLetter(str.charAt(idx)))
                  temp.append(str.charAt(idx));
          str = temp.toString();
        */
    }

    private void toMap(){
        String[] words = str.split(" ");

        for(String word : words) {

            for(int idx = 0; idx < word.length(); idx++) {
                char currChar = word.charAt(idx);

                if( map.containsKey(currChar) ) {
                    map.get(currChar).push(word);
                }else{
                    info newInfo = new info(word);
                    map.put(currChar, newInfo);
                    mapSize++;
                }

            }

        }
    }

    private void printOriginalMap(){
        System.out.println("The original (unsorted) map: ");
        for(Map.Entry<Character,info> entry : map.entrySet()) {
            Character key = entry.getKey();
            info value = entry.getValue();

            System.out.println("Letter: "+key+" - Count: " + value.getCount()+ " - Words: " + value.getWords());
        }
    }

    public LinkedHashMap<Character, info> getMap() {
        return map;
    }
    public void setMap(LinkedHashMap<Character, info> map) {
        this.map = map;
    }

    public int getMapSize(){return mapSize;}
    public String getStr(){return str;}
}
