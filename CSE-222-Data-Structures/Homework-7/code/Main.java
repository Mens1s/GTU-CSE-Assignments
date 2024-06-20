public class Main {
    public static void main(String[] args) {
        /*
        myMap mp = new myMap("'Hush, hush!' whispered the rushing wind.");
        mp.doOperations();

        mp = new myMap("Buzzing bees buzz.");
        mp.doOperations();
           */
        System.out.println("BEST STRING: ");
        myMap mp = new myMap("a b b c c c d d d d e e e e e f f f f f f g g g g g g g h h h h h h h h i i i i i i i i i j j j j j j j j j j k k k k k k k k k k k k");
        mp.doOperations();

        System.out.println("AVERAGE-CASE STRING: ");
        myMap mp1 = new myMap("a b b c c c d d d d e e e e e f f f f f f g g g g g g g h h h h h h h h k k k k k k k k k k k k j j j j j j j j j j i i i i i i i i i");
        mp1.doOperations();

        System.out.println("WORST-CASE STRING: ");
        myMap mp2 = new myMap("k k k k k k k k k k k k j j j j j j j j j j i i i i i i i i i h h h h h h h h g g g g g g g f f f f f f e e e e e d d d d c c c b b a");
        mp2.doOperations();


    }
}