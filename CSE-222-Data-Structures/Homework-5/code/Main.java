import java.util.Scanner;

public class Main {
    /**
     * main method
     * It includes menu to user choice*/
    public static void main(String[] args) {
        TreeFrameBuilder d = new TreeFrameBuilder();
        /*
        2021;CSE102;LECTURE1
        2022;CSE321;LECTURE1
        2022;CSE321;LECTURE2
        2023;CSE222;LECTURE1;PROBLEM1
        2023;CSE222;LECTURE1;PROBLEM2
        2023;CSE232;LECTURE1
        2023;CSE232;LECTURE2;PROBLEM1
        2023;CSE232;LECTURE2;PROBLEM2
        2023;CSE232;LECTURE3
        */
        Scanner scanner = new Scanner(System.in);
        String input = "";
        System.out.println("Welcome to our system.");
        while (!input.equals("exit")) {
            System.out.println("\n1- BFS Search\n2- DFS Search \n3- Traverse Search\n4- Move Directory <from to>\n exit for determine the app");
            input = scanner.next();

            if(input.equals("1")){
                System.out.println("Enter TARGET VALUE : ");
                input = scanner.next();
                d.BFS(input);
            }if(input.equals("2")){
                System.out.println("Enter TARGET VALUE : ");
                input = scanner.next();
                d.DFS(input);
            }if(input.equals("3")){
                System.out.println("Enter TARGET VALUE : ");
                input = scanner.next();
                d.Traverse(input);
            }if(input.equals("4")){
                System.out.println("OUR splitter is -> so enter like 2023->CSE321->LECTURE2");

                System.out.println("Enter FROM path :");
                String from = scanner.next();

                System.out.println("Enter TO path :");
                String to = scanner.next();
                
                d.Move(from, to);
            }
        }
        /*
        System.out.println("Are you sure to move 2022->CSE321->LECTURE2 to 2023 file? Enter something to continue!");
        scanner.next();
        d.Move("2022->CSE321->LECTURE2", "2023");

        d = new TreeFrameBuilder();
        System.out.println("Are you sure to move 2022->CSE321 to 2020 file? Enter something to continue!");
        scanner.next();
        d.Move("2022->CSE321", "2020");

        d = new TreeFrameBuilder();
        System.out.println("Are you sure to move 2022->CSE222 to 2020 file? Enter something to continue!");
        scanner.next();
        d.Move("2022->CSE222", "2020");

        d = new TreeFrameBuilder();
        System.out.println("Are you sure to move 2023->CSE222->LECTURE1->PROBLEM2 to 2022 file? Enter something to continue!");
        scanner.next();
        d.Move("2023->CSE222->LECTURE1->PROBLEM2", "2022");
        */

        /*
        2021;CSE102;LECTURE1
        2022;CSE321;LECTURE1
        2022;CSE321;LECTURE2
        2023;CSE321;LECTURE1
        2023;CSE222;LECTURE1;PROBLEM1
        2023;CSE222;LECTURE1;PROBLEM2
        2023;CSE232;LECTURE1
        2023;CSE232;LECTURE2;PROBLEM1
        2023;CSE232;LECTURE2;PROBLEM2
        2023;CSE232;LECTURE3
        */
        //d.Move("2023->CSE321->LECTURE1", "2022");

    }
}