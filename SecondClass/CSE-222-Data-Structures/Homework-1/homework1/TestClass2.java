package homework1;

public class TestClass2 {
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

        //accounts[2].viewProfile(accounts, "sibelgulmez");

        //accounts[2].viewPosts(accounts);

        accounts[2].likePost(accounts,1);

        accounts[2].commentPost(accounts, 0, "said 'me too!");

        accounts[2].follow(accounts, "sibelgulmez");
        accounts[2].follow(accounts, "gizemsungu");

        accounts[2].sendMessage(accounts, "gizemsungu", "This homework so easy...");

        accounts[2].logout();

        accounts[0].login(accounts);

        //accounts[0].showCountOutbox();

        //accounts[0].showCountInbox();

        //accounts[0].showInbox(accounts);

        //accounts[0].viewProfile(accounts, "sibelgulmez");

        //accounts[0].viewPosts(accounts);

        //accounts[0].viewPostsInteractions(accounts);

        accounts[0].likePost(accounts, 0);
        accounts[0].likePost(accounts, 1);

        //accounts[0].viewPostsInteractions(accounts);
        /* TEST DRIVER 1 end */
        /* STARTING OF TEST DRIVER 2 */
        System.out.println("\nBefore steps done exactly same as TEST DRIVER 1...\nStarting TEST DRIVER 2 with STEP 23");

        System.out.println("\nStep 23... gizemsungu logs in...");
        accounts[0].login(accounts);

        System.out.println("\nStep 23... Sharing post...");
        accounts[0].sharePost(accounts,"Post1");

        System.out.println("\nStep 24... Sharing post...");
        accounts[0].sharePost(accounts,"Post2");

        System.out.println("\nStep 25... gizemsungu logs out...");
        accounts[0].logout();

        System.out.println("\nStep 26... sibelgulmez logs in...");
        accounts[1].login(accounts);

        System.out.println("\nStep 27... sibelgulmez views gizemsungu profile...");
        accounts[1].viewProfile(accounts, "gizemsungu");

        System.out.println("\nStep 28... sibelgulmez likes post1...");
        accounts[1].likePost(accounts, 2);

        System.out.println("\nStep 29... sibelgulmez logs out...");
        accounts[1].logout();

        System.out.println("\nStep 30... gokhankaya log in...");
        accounts[2].login(accounts);

        System.out.println("\nStep 31... gokhankaya views gizemsungu profile...");
        accounts[2].viewProfile(accounts, "gizemsungu");

        System.out.println("\nStep 32... gokhankaya comments post2 profile...");
        accounts[2].commentPost(accounts, 3, "Nice!");

        System.out.println("\nStep 33... gokhankaya sends message to gizemsungu Hello! ...");
        accounts[2].sendMessage(accounts, "gizemsungu", "Hello!");

        System.out.println("\nStep 34... gokhankaya logs out...");
        accounts[2].logout();

        System.out.println("\nStep 35... gizemsungu logs in...");
        accounts[0].login(accounts);

        System.out.println("\nStep 36... gizemsungu views her own profile...");
        accounts[0].viewProfile(accounts, "gizemsungu");

        System.out.println("\nStep 37... gizemsungu reads message from gokhankaya...");
        accounts[0].showInbox(accounts, "gokhankaya");
    }
}