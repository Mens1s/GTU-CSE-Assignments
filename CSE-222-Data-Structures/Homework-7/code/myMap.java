import java.util.LinkedHashMap;
import java.util.Map;
import java.time.Duration;
import java.time.Instant;
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
        /* TESTS ALL */
        
        bubbleSort b1 = new bubbleSort(this);
        insertionSort i1 = new insertionSort(this);
        mergeSort m1 = new mergeSort(this);
        quickSort q1 = new quickSort(this);
        selectionSort s1 = new selectionSort(this);

        b1.sort();
        i1.sort();
        m1.sort();
        q1.sort();
        s1.sort();

        b1.printSortedMap();
        i1.printSortedMap();
        m1.printSortedMap();
        q1.printSortedMap();
        s1.printSortedMap();


        /* END OF TESTS ALL */
        /* Time Complexity Analysis */
        /*
        // MERGE SORT
        System.out.println("\n\nThe sorted map using mergeSort: ");

        mergeSort sort = new mergeSort(this);

        Instant start = Instant.now();
        sort.sort();
        Instant end = Instant.now();
        Duration timeElapsed = Duration.between(start, end);
        System.out.println("MERGE SORT Time taken: "+ timeElapsed.toNanos() +" milliseconds");

        //sort.printSortedMap();

        // SELECTION SORT
        System.out.println("\n\nThe sorted map using selectionSort: ");
        selectionSort selectionSort1 = new selectionSort(this);

        start = Instant.now();
        selectionSort1.sort();
        end = Instant.now();

        timeElapsed = Duration.between(start, end);
        System.out.println("SELECTION SORT Time taken: "+ timeElapsed.toNanos() +" milliseconds");

        //selectionSort1.printSortedMap();

        // INSERTION SORT
        System.out.println("\n\nThe sorted map using insertionSort: ");
        insertionSort insertionSort1 = new insertionSort(this);

        start = Instant.now();
        insertionSort1.sort();
        end = Instant.now();

        timeElapsed = Duration.between(start, end);
        System.out.println("INSERTION SORT Time taken: "+ timeElapsed.toNanos() +" milliseconds");

        //insertionSort1.printSortedMap();

        // BUBBLE SORT
        System.out.println("\n\nThe sorted map using bubbleSort: ");
        bubbleSort bubbleSort1 = new bubbleSort(this);

        start = Instant.now();
        bubbleSort1.sort();
        end = Instant.now();

        timeElapsed = Duration.between(start, end);
        System.out.println("BUBBLE SORT Time taken: "+ timeElapsed.toNanos() +" milliseconds");
        //bubbleSort1.printSortedMap();

        // QUICK SORT
        System.out.println("\n\nThe sorted map using quickSort: ");
        quickSort quickSort1 = new quickSort(this);

        start = Instant.now();
        quickSort1.sort();
        end = Instant.now();

        timeElapsed = Duration.between(start, end);
        System.out.println("QUICK SORT Time taken: "+ timeElapsed.toNanos() +" milliseconds");

        quickSort1.printSortedMap();
        */
    }

    private void preProcesses(){
        str = str.toLowerCase().replaceAll("[^a-z\\s]", "");
        boolean flag = false;

        for(char c : str.toCharArray())
            if(Character.isLetter(c))
                flag = true;

        if(!flag) str = "";

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
