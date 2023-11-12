package homework1;

public class TestClass1 {
    public static void main(String[] args) {


        Account [] accounts = new Account[3];
        int accountCount = 0;

        System.out.println("\nStep 1... Creating accounts...");
        try {
            System.out.println("An account with username " + "gizemsungu" + " has been created.");
            accounts[0] = new Account(accountCount++, "gizemsungu", "21/01/1990", "Ankara", accounts);

            System.out.println("An account with username " + "sibelgulmez" + " has been created.");
            accounts[1] = new Account(accountCount++, "sibelgulmez", "12/12/1980", "Istanbul", accounts);

            System.out.println("An account with username " + "gokhankaya" + " has been created.");
            accounts[2] = new Account(accountCount++, "gokhankaya", "11/05/1994", "Antalya", accounts);
        }
        catch (Exception e){
            System.out.println(e.getMessage());
        }

        System.out.println("\nStep 2... Logging into an account (username: sibelgulmez)...");
        accounts[1].login(accounts);

        System.out.println("\nStep 3... Sharing two posts...");
        accounts[1].sharePost(accounts, "I like the java...");
        accounts[1].sharePost(accounts, "Java the coffee..");

        System.out.println("\nStep 4... Following gizemsungu and gokhankaya...");
        accounts[1].follow(accounts, "gizemsungu");
        accounts[1].follow(accounts, "gokhankaya");

        System.out.println("\nStep 5... Logging out from account 'sibelgulmez'... ");
        accounts[1].logout();

        System.out.println("\nStep 6... Logging into another account (username: gokhankaya)...");
        accounts[2].login(accounts);

        System.out.println("\nStep 7... Viewing sibelgulmez's profile...");
        accounts[2].viewProfile(accounts, "sibelgulmez");

        System.out.println("\nStep 8... Viewing sibelgulmez's posts");
        accounts[2].viewPosts(accounts);

        System.out.println("\nStep 9... Liking a post of sibelgulmez...");
        accounts[2].likePost(accounts,0);

        System.out.println("\nStep 10... Adding a comment on a post of sibelgulmez...");
        accounts[2].commentPost(accounts, 0, "said 'me too!");

        System.out.println("\nStep 11... Following sibelgulmez and gizemsungu..");
        accounts[2].follow(accounts, "sibelgulmez");
        accounts[2].follow(accounts, "gizemsungu");

        System.out.println("\nStep 12... Sending a message to gizemsungu");
        accounts[2].sendMessage(accounts, "gizemsungu", "This homework so easy...");

        System.out.println("\nStep 13... Logging out from account 'gokhankaya'... ");
        accounts[2].logout();

        System.out.println("\nStep 14... Logging into another account (username: gizemsungu)...");
        accounts[0].login(accounts);

        System.out.println("\nStep 15... Checking outbox...");
        accounts[0].showCountOutbox();

        System.out.println("\nStep 16... Checking inbox...");
        accounts[0].showCountInbox();

        System.out.println("\nStep 17... Viewing inbox...");
        accounts[0].showInbox(accounts);

        System.out.println("\nStep 18... Viewing sibelgulmez's profile...");
        accounts[0].viewProfile(accounts, "sibelgulmez");

        System.out.println("\nStep 19... Viewing sibelgulmez's posts...");
        accounts[0].viewPosts(accounts);

        System.out.println("\nStep 20... Viewing sibelgulmez's posts interactions...");
        accounts[0].viewPostsInteractions(accounts);

        System.out.println("\nStep 21... Liking sibelgulmez's posts...");
        accounts[0].likePost(accounts, 0);
        accounts[0].likePost(accounts, 1);

        System.out.println("\nStep 22... Viewing sibelgulmez's posts interactions...");
        accounts[0].viewPostsInteractions(accounts);

    }
}