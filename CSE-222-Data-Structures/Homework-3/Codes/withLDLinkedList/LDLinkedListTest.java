package withLDLinkedList;

public class LDLinkedListTest {
    public static void main(String[] args) {

        LDLinkedList<Integer> list = new LDLinkedList<>();

        list.add(0);
        list.add(1);
        list.add(2);
        list.add(3);
        list.add(4);
        list.add(5);
        list.add(6);
        list.add(7);
        list.add(8);
        list.add(9);
        for(Integer num : list) System.out.print(num + ", ");


        System.out.println("\n GET 2 IDX Number : "+list.get(2));
        list.remove(2);

        System.out.println("\n GET 2 IDX Number : "+list.get(2));

        list.set(2,99);

        System.out.println("\n GET 2 IDX Number : "+list.get(2));

        System.out.println("\n GET SIZE : "+list.size());

        for(Integer num : list) System.out.print(num + ", "); // logical list
        System.out.println("\n"+list); // physical list
    }
}
