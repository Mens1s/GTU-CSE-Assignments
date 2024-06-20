package withLinkedList;
import java.util.LinkedList;

public class TestClass3 {
    public static void main(String[] args) {

        LinkedList<Account> accounts = new LinkedList<>();
        int accountCount = 0;
        /* TEST DRIVER 1 start */
        try {
            accounts.add(new Account(accountCount++, "gizemsungu", "21/01/1990", "Ankara", accounts));

            accounts.add(new Account(accountCount++, "sibelgulmez", "12/12/1980", "Istanbul", accounts));

            accounts.add(new Account(accountCount++, "gokhankaya", "11/05/1994", "Antalya", accounts));

        }
        catch (Exception e){
            System.out.println(e.getMessage());
        }
        
        accounts.get(1).login(accounts);

        accounts.get(1).sharePost(accounts, "I like the java...");
        accounts.get(1).sharePost(accounts, "Java the coffee..");

        accounts.get(1).follow(accounts, "gizemsungu");
        accounts.get(1).follow(accounts, "gokhankaya");

        accounts.get(1).logout();

        accounts.get(2).login(accounts);

        accounts.get(2).viewProfile(accounts, "sibelgulmez");

        accounts.get(2).viewPosts(accounts);

        accounts.get(2).likePost(accounts, 1);

        accounts.get(2).commentPost(accounts, 0, "said 'me too!");

        accounts.get(2).follow(accounts, "sibelgulmez");
        accounts.get(2).follow(accounts, "gizemsungu");

        accounts.get(2).sendMessage(accounts, "gizemsungu", "This homework so easy...");

        accounts.get(2).logout();

        accounts.get(0).login(accounts);

        accounts.get(0).showCountOutbox();

        accounts.get(0).showCountInbox();

        accounts.get(0).showInbox(accounts);

        accounts.get(0).viewProfile(accounts, "sibelgulmez");

        accounts.get(0).viewPosts(accounts);

        accounts.get(0).viewPostsInteractions(accounts);

        accounts.get(0).likePost(accounts, 0);
        accounts.get(0).likePost(accounts, 1);
        /* TEST DRIVER 1 end */
        /* STARTING OF TEST DRIVER 3 */
        System.out.println("\nBefore steps done exactly same as TEST DRIVER 1...\nStarting TEST DRIVER 3 with STEP 23");

        System.out.println("\nStep 23... gizemsungu logs in...");
        accounts.get(0).login(accounts);

        System.out.println("\nStep 24... gizemsungu blocks sibelgulmez...");
        accounts.get(0).blockUsername(accounts, "sibelgulmez");

        System.out.println("\nStep 25... gizemsungu logs out...");
        accounts.get(0).logout();

        System.out.println("\nStep 26... sibelgulmez log in...");
        accounts.get(1).login(accounts);

        System.out.println("\nStep 27... sibelgulmez tries to view the profile of 'gizemsungu' ...");
        accounts.get(1).viewProfile(accounts, "gizemsungu");

        System.out.println("\nStep 28... sibelgulmez tries to send message to 'gizemsungu' ...");
        accounts.get(1).sendMessage(accounts, "gizemsungu", "helllooo");

    }
}
