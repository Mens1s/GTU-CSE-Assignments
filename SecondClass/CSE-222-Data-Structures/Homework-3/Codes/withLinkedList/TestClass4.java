package withLinkedList;

import java.util.LinkedList;

public class TestClass4 {
    public static void main(String[] args) {

        LinkedList<Account> accounts = new LinkedList<>();
        int accountCount = 0;

        System.out.println("\nStep 1... Creating accounts...");
        try {
            System.out.println("An account with username " + "a1" + " has been created.");
            accounts.add(new Account(accountCount++, "a1", "21/01/1990", "Ankara", accounts));

            System.out.println("An account with username " + "a2" + " has been created.");
            accounts.add(new Account(accountCount++, "a2", "12/12/1980", "Istanbul", accounts));

            System.out.println("An account with username " + "a3" + " has been created.");
            accounts.add(new Account(accountCount++, "a3", "11/04/1994", "Antalya", accounts));

            System.out.println("An account with username " + "a4" + " has been created.");
            accounts.add(new Account(accountCount++, "a4", "12/03/1995", "Ankara", accounts));

            System.out.println("An account with username " + "a5" + " has been created.");
            accounts.add(new Account(accountCount++, "a5", "14/02/1996", "Mus", accounts));

            System.out.println("An account with username " + "a6" + " has been created.");
            accounts.add(new Account(accountCount++, "a6", "21/05/1924", "Gaziantep", accounts))
            ;
            System.out.println("An account with username " + "a7" + " has been created.");
            accounts.add(new Account(accountCount++, "a7", "23/05/1833", "Trabzon", accounts));

            System.out.println("An account with username " + "a8" + " has been created.");
            accounts.add(new Account(accountCount++, "a8", "15/12/1999", "Samsun", accounts));

            System.out.println("An account with username " + "a9" + " has been created.");
            accounts.add(new Account(accountCount++, "a9", "07/11/2001", "Izmir", accounts));

            System.out.println("An account with username " + "a10" + " has been created.");
            accounts.add(new Account(accountCount++, "a10", "01/07/2002", "Eskisehir", accounts));

        }
        catch (Exception e){
            System.out.println(e.getMessage());
        }
        /*
        a1 login
        a1 post post1, post2, post3
        a1 follow a2,a6,a9,a10
        a1 logout
        */
        accounts.get(0).login(accounts);

        accounts.get(0).sharePost(accounts,"post1");
        accounts.get(0).sharePost(accounts,"post2");
        accounts.get(0).sharePost(accounts,"post3");

        accounts.get(0).follow(accounts,"a2");
        accounts.get(0).follow(accounts,"a6");
        accounts.get(0).follow(accounts,"a9");
        accounts.get(0).follow(accounts,"a10");

        accounts.get(0).logout();
        /*
        a2 login
        a2 follow a1, a3,a5,a6,a10,a9
        a2 view a1
        a2 like post1, post2
        a2 comment post1 a2 first comment
        a2 comment post3 a2 second comment
        a2 send message a1 hi a1 wbu?
        a2 logout
        */
        accounts.get(1).login(accounts);

        accounts.get(1).follow(accounts, "a1");
        accounts.get(1).follow(accounts, "a3");
        accounts.get(1).follow(accounts, "a5");
        accounts.get(1).follow(accounts, "a6");
        accounts.get(1).follow(accounts, "a10");
        accounts.get(1).follow(accounts, "a9");

        accounts.get(1).viewProfile(accounts,"a1");

        accounts.get(1).likePost(accounts,0);
        accounts.get(1).likePost(accounts,1);

        accounts.get(1).commentPost(accounts,0,"a2 first comment");
        accounts.get(1).commentPost(accounts,1,"a2 second comment");

        accounts.get(1).sendMessage(accounts,"a1","hi a1 wbu?");
        accounts.get(1).logout();
        /*
        a3 login
        a3 follow a1,a4,a5,a6
        a3 view a1
        a3 like post1, post2, post3
        a3 comment post3 a3 third comment
        a3 send message a1 hi a1?
        a3 logout
        */
        accounts.get(2).login(accounts);

        accounts.get(2).follow(accounts,"a1");
        accounts.get(2).follow(accounts,"a4");
        accounts.get(2).follow(accounts,"a5");
        accounts.get(2).follow(accounts,"a6");

        accounts.get(2).viewProfile(accounts,"a1");

        accounts.get(2).likePost(accounts,0);
        accounts.get(2).likePost(accounts,1);
        accounts.get(2).likePost(accounts,2);

        accounts.get(2).commentPost(accounts, 2,"a3 third comment");

        accounts.get(2).sendMessage(accounts, "a1","h1 a1 it is me a3");

        accounts.get(2).logout();
        /*
        a4 login
        a4 follow a1,a2,a4,a5,a6,a7,a8,a9
        a4 view a1
        a4 like post1, post3
        a4 post post4
        a4 logout
        */
        accounts.get(3).login(accounts);

        accounts.get(3).follow(accounts,"a1");
        accounts.get(3).follow(accounts,"a2");
        accounts.get(3).follow(accounts,"a4");
        accounts.get(3).follow(accounts,"a5");
        accounts.get(3).follow(accounts,"a6");
        accounts.get(3).follow(accounts,"a7");
        accounts.get(3).follow(accounts,"a8");
        accounts.get(3).follow(accounts,"a9");

        accounts.get(3).viewProfile(accounts,"a1");

        accounts.get(3).likePost(accounts,0);
        accounts.get(3).likePost(accounts,2);

        accounts.get(3).sharePost(accounts,"post4");

        accounts.get(3).logout();
        /*
        a5 login
        a5 follow a1,a2,a4,a6,a7,a8,a9
        a5 view a1
        a5 like post1 post2
        a5 view a4
        a5 like post4
        a5 comment post4 is goood
        a5 send message a4 hello a4 it is me a5
        a5 logout
        */
        accounts.get(4).login(accounts);

        accounts.get(4).follow(accounts,"a1");
        accounts.get(4).follow(accounts,"a2");
        accounts.get(4).follow(accounts,"a4");
        accounts.get(4).follow(accounts,"a6");
        accounts.get(4).follow(accounts,"a7");
        accounts.get(4).follow(accounts,"a8");
        accounts.get(4).follow(accounts,"a9");

        accounts.get(4).viewProfile(accounts, "a1");
        accounts.get(4).likePost(accounts,0);
        accounts.get(4).likePost(accounts,1);

        accounts.get(4).viewProfile(accounts, "a4");
        accounts.get(4).likePost(accounts,3);
        accounts.get(4).commentPost(accounts,  3,"post 4 is good");

        accounts.get(4).sendMessage(accounts,"a4","a4 it is me a5");

        accounts.get(4).logout();
        /*
        a6 login
        a6 follow a1,a2,a4,a6,a7,a8,a9
        a6 view a1
        a6 like post1 post3
        a6 view a4
        a6 like post4
        a6 comment post4 that is nice picture
        a6 logout
        */
        accounts.get(5).login(accounts);

        accounts.get(5).follow(accounts,"a1");
        accounts.get(5).follow(accounts,"a2");
        accounts.get(5).follow(accounts,"a4");
        accounts.get(5).follow(accounts,"a6");
        accounts.get(5).follow(accounts,"a7");
        accounts.get(5).follow(accounts,"a8");
        accounts.get(5).follow(accounts,"a9");

        accounts.get(5).viewProfile(accounts,"a1");
        accounts.get(5).likePost(accounts,0);
        accounts.get(5).likePost(accounts,2);

        accounts.get(5).viewProfile(accounts,"a4");
        accounts.get(5).likePost(accounts,3);
        accounts.get(5).commentPost(accounts,3,"that is nice picture, it is a6");

        accounts.get(5).logout();
        /*
        a7 login
        a7 follow a1,a2,a6,a4,a8,a9
        a7 view a1
        a7 like post2, post1
        a7 send message a1 hi it is me a7
        a7 comment post1 hi wbu
        a7 logout
        */
        accounts.get(6).login(accounts);

        accounts.get(6).follow(accounts,"a1");
        accounts.get(6).follow(accounts,"a2");
        accounts.get(6).follow(accounts,"a6");
        accounts.get(6).follow(accounts,"a4");
        accounts.get(6).follow(accounts,"a8");
        accounts.get(6).follow(accounts,"a9");

        accounts.get(6).viewProfile(accounts,"a1");
        accounts.get(6).likePost(accounts,1);
        accounts.get(6).likePost(accounts,0);

        accounts.get(6).sendMessage(accounts,"a1","hi it is ne a7");

        accounts.get(6).commentPost(accounts,0,"hi wbu, it is account 7");

        accounts.get(6).logout();

        /*
        a8 login
        a8 follow a1,a2,a6,a7,a10
        a8 view a1
        a8 like post2
        a8 logout
        */
        accounts.get(7).login(accounts);

        accounts.get(7).follow(accounts,"a1");
        accounts.get(7).follow(accounts,"a2");
        accounts.get(7).follow(accounts,"a6");
        accounts.get(7).follow(accounts,"a7");
        accounts.get(7).follow(accounts,"a10");

        accounts.get(7).viewProfile(accounts,"a1");
        accounts.get(7).likePost(accounts,1);

        accounts.get(7).logout();

        /*
        a9 login
        a9 follow a1,a2,a4,a6,a7,a8
        a9 view a1
        a9 like post1 post2
        a9 view a4
        a9 like post4
        a9 comment post4 is goood postt
        a9 logout
        */
        accounts.get(8).login(accounts);

        accounts.get(8).follow(accounts,"a1");
        accounts.get(8).follow(accounts,"a2");
        accounts.get(8).follow(accounts,"a4");
        accounts.get(8).follow(accounts,"a5");
        accounts.get(8).follow(accounts,"a7");
        accounts.get(8).follow(accounts,"a8");

        accounts.get(8).viewProfile(accounts, "a1");
        accounts.get(8).likePost(accounts,0);
        accounts.get(8).likePost(accounts,1);

        accounts.get(8).viewProfile(accounts, "a4");
        accounts.get(8).likePost(accounts,3);
        accounts.get(8).commentPost(accounts,3,"post4 is goood postt it is me a9");

        accounts.get(8).logout();

        /*
        a10 login
        a10 follow a1
        a10 follow a4

        a10 view a1
        a10 view post1
        a10 view post2
        a10 view post3

        a10 view a4
        a10 logout
        */
        accounts.get(9).login(accounts);

        accounts.get(9).follow(accounts,"a1");
        accounts.get(9).follow(accounts,"a4");

        System.out.println("******************************************");
        System.out.println("TABLE CONTROL");
        System.out.println("******************************************");

        System.out.println("[a10] Viewing account a1...");
        accounts.get(9).viewProfile(accounts,"a1");
        accounts.get(9).viewPostsInteractions(accounts);

        System.out.println("[a10] Viewing account a4...");
        accounts.get(9).viewProfile(accounts,"a4");
        accounts.get(9).viewPostsInteractions(accounts);

        accounts.get(9).logout();

        /* testing unlike and uncomment */
        System.out.println("******************************************");
        System.out.println("TESTING UNLIKE AND UNCOMMENT");
        System.out.println("******************************************");

        System.out.println("\n***Login as account 7 ***\n");
        accounts.get(6).login(accounts);

        System.out.println("\n***[account7] viewing a1, and post interactions ***\n");
        accounts.get(6).viewProfile(accounts,"a1");
        accounts.get(6).viewPostsInteractions(accounts);

        System.out.println("\n***[account7] unliking post2, and viewing post interactions ***\n");
        accounts.get(6).unlikePost(accounts,1);
        accounts.get(6).viewPostsInteractions(accounts);


        System.out.println("\n***[account7] uncomment post1, and viewing post interactions ***\n");
        accounts.get(6).uncommentPost(accounts,0);
        accounts.get(6).viewPostsInteractions(accounts);

        accounts.get(6).logout();

        /* testing unfollow */
        System.out.println("******************************************");
        System.out.println("TESTING UNFOLLOW FUNCTION");
        System.out.println("******************************************");

        System.out.println("\n***Login as account 3 ***\n");
        accounts.get(2).login(accounts);

        System.out.println("\n***[account3] viewing a1 ***\n");
        accounts.get(2).viewProfile(accounts,"a1");

        System.out.println("\n***[account3] unfollow a1 ***\n");
        accounts.get(2).unfollow(accounts,"a1");

        System.out.println("\n***[account3] viewing a1 ***\n");
        accounts.get(2).viewProfile(accounts,"a1");

        accounts.get(2).logout();

        /* testing BLOCK */
        System.out.println("******************************************");
        System.out.println("TESTING BLOCK FUNCTION");
        System.out.println("******************************************");
        System.out.println("\n***Login as account 1 ***\n");
        accounts.get(0).login(accounts);

        System.out.println("\n***[account1] viewing a1 ***\n");
        accounts.get(0).viewProfile(accounts,"a1");

        System.out.println("\n***[account1] viewing a1 post interactions ***\n");
        accounts.get(0).viewPostsInteractions(accounts);

        System.out.println("\n***[account1] blocking a2 ***\n");
        accounts.get(0).blockUsername(accounts,"a2");

        System.out.println("\n***[account1] viewing a1 ***\n");
        accounts.get(0).viewProfile(accounts,"a1");

        System.out.println("\n***[account1] viewing a1 post interactions ***\n");
        accounts.get(0).viewPostsInteractions(accounts);

        accounts.get(0).logout();

        System.out.println("\n***Login as account 2 ***\n");
        accounts.get(1).login(accounts);

        System.out.println("\n***[account2] viewing a2 ***\n");
        accounts.get(1).viewProfile(accounts,"a2");

        System.out.println("\n***[account2] trying to view a1 ***\n");
        accounts.get(1).viewProfile(accounts,"a1");

        accounts.get(1).logout();

        /* testing UNBLOCK */
        System.out.println("******************************************");
        System.out.println("TESTING UNBLOCK FUNCTION");
        System.out.println("******************************************");

        System.out.println("\n***Login as account 1 ***\n");
        accounts.get(0).login(accounts);

        System.out.println("\n***[account1] unblocking a2 ***\n");
        accounts.get(0).unBlockUsername(accounts,"a2");

        accounts.get(0).logout();

        System.out.println("\n***Login as account 2 ***\n");
        accounts.get(1).login(accounts);

        System.out.println("\n***[account2] trying to view a1 ***\n");
        accounts.get(1).viewProfile(accounts,"a1");

        accounts.get(1).logout();

    }
}
