package homework1;

public class TestClass3 {
    public static void main(String[] args) {

        Account [] accounts = new Account[3];
        int accountCount = 0;
        /* TEST DRIVER 1 start */
        try {
            accounts[0] = new Account(accountCount++, "gizemsungu", "21/01/1990", "Ankara", accounts);
            accounts[1] = new Account(accountCount++, "sibelgulmez", "12/12/1980", "Istanbul", accounts);
            accounts[2] = new Account(accountCount++, "gokhankaya", "11/05/1994", "Antalya", accounts);
        }
        catch (Exception e){
            System.out.println(e.getMessage());
        }
        accounts[1].login(accounts);

        accounts[1].sharePost(accounts, "I like the java...");
        accounts[1].sharePost(accounts, "Java the coffee..");

        accounts[1].follow(accounts, "gizemsungu");
        accounts[1].follow(accounts, "gokhankaya");

        accounts[1].logout();

        accounts[2].login(accounts);

        accounts[2].viewProfile(accounts, "sibelgulmez");
        
        accounts[2].viewPosts(accounts);

        accounts[2].likePost(accounts,1);

        accounts[2].commentPost(accounts, 0, "said 'me too!");

        accounts[2].follow(accounts, "sibelgulmez");
        accounts[2].follow(accounts, "gizemsungu");

        accounts[2].sendMessage(accounts, "gizemsungu", "This homework so easy...");

        accounts[2].logout();

        accounts[0].login(accounts);

        accounts[0].showCountOutbox();

        accounts[0].showCountInbox();

        accounts[0].showInbox(accounts);

        accounts[0].viewProfile(accounts, "sibelgulmez");

        accounts[0].viewPosts(accounts);

        accounts[0].viewPostsInteractions(accounts);

        accounts[0].likePost(accounts, 0);
        accounts[0].likePost(accounts, 1);
        /* TEST DRIVER 1 end */
        /* STARTING OF TEST DRIVER 3 */
        System.out.println("\nBefore steps done exactly same as TEST DRIVER 1...\nStarting TEST DRIVER 3 with STEP 23");

        System.out.println("\nStep 23... gizemsungu logs in...");
        accounts[0].login(accounts);

        System.out.println("\nStep 24... gizemsungu blocks sibelgulmez...");
        accounts[0].blockUsername(accounts, "sibelgulmez");

        System.out.println("\nStep 25... gizemsungu logs out...");
        accounts[0].logout();

        System.out.println("\nStep 26... sibelgulmez log in...");
        accounts[1].login(accounts);

        System.out.println("\nStep 27... sibelgulmez tries to view the profile of 'gizemsungu' ...");
        accounts[1].viewProfile(accounts, "gizemsungu");

        System.out.println("\nStep 28... sibelgulmez tries to send message to 'gizemsungu' ...");
        accounts[1].sendMessage(accounts, "gizemsungu", "helllooo");


    }
}