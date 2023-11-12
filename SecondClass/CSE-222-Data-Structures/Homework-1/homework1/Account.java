package homework1;

/**
 * The homework1.Account Class implements account object.
 * homework1.Account objects holds unique account id, unique username, birthdate, location, post-message-follower-following-block arrays, current status
 * homework1.Account objects can like - comment post whose owner followed, follow - block - send message to, another account object, following by another account object,
 *
 * @author Ahmet / mens1s
 * @version 0.1
 * @since 2023-03-17
 *
 */
public class Account {
    /**
     * unique id of each homework1.Account object */
    private int accountId;
    /**
     * unique username of each homework1.Account object */
    private String username;
    /**
     * birthdate of homework1.Account object */
    private String birthdate;
    /**
     * location of homework1.Account object */
    private String location;
    /**
     * posts array holds all posted object */
    private Post[] posts;
    /**
     * messages array holds all messages object*/
    private Message[] messages;
    /**
     * following array holds all following account with usernames */
    private String[] following;
    /**
     * followers array holds all followers account with usernames */
    private String[] followers;
    /**
     * blockedList array holds all blocked account with usernames */
    private String[] blockedList;
    /**
     * currentStatus hold account status is viewing profile [which profile with username], is login, is logout,  */
    private String currentStatus; // this variable approved by t.assistant on ps

    /**
     * Constructor of homework1.Comment class
     * @param accountId INT unique id for each account object
     * @param username STRING unique username of account object
     * @param birthdate STRING birthdate of account
     * @param location STRING location of account
     * @param accounts ACCOUNT ARRAY all created account objects*/
    public Account(int accountId, String username, String birthdate, String location, Account [] accounts) throws Exception {
        // checking of username is already used or not
        //try{
        for(Account account : accounts){
            if(account == null) break;
            if(account.username == username)
                throw new Exception("Username Exception");
        }

        this.accountId = accountId;
        this.username = username;
        this.birthdate = birthdate;
        this.location = location;

        posts = new Post[100];
        messages = new Message[100];

        this.blockedList = new String[100];
        this.following = new String[100];
        this.followers = new String[100];

        currentStatus = "logout";
    }

    /* profile field */
        /* block functions field */
    /**
     * blockUsername function block account object if available also unfollow two sides
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * @param blockedUsername STRING that blocked
     * @return INT -1 no account find with given username, 1 blocked successfully
     * */
    public int blockUsername(Account [] accounts, String blockedUsername){
        boolean flag = false;
        for(Account account : accounts){
            if(account == null) break;
            else if( account.username == blockedUsername) {
                // delete from following - followers list of blocked account
                account.deleteUserFromFollowers(getUsername());
                account.deleteUserFromFollowing(getUsername());
                flag = true;
            }
        }
        if(flag){
            blockedList[getBlockedCount()] = blockedUsername;
            // delete from following - followers list who blocked account
            deleteUserFromFollowers(blockedUsername);
            deleteUserFromFollowing(blockedUsername);
            return 1;
        }
        System.out.println("There is no user as a " + blockedUsername);
        return -1;
    }
    /**
     * getBlockedCount function return number of blocked account object
     * @return INT number of blocked account object
     * */
    private int getBlockedCount() {
        int counter = 0;
        for (String u : getBlockedList()){
            if (u == null) return counter;
            counter++;
        }
        return counter;
    }
    /**
     * isBlocked function return account with given username is blocked or not
     * @param viewedProfileUsername STRING username that checks blocked or not
     * @return BOOLEAN account with given username is blocked or not
     * */
    private boolean isBlocked(String viewedProfileUsername){
        for(String username : getBlockedList()){
            if(username == null) break;
            if(username == viewedProfileUsername) return true;
        }
        return false;
    }
    /**
     * deleteUserFromFollowers function deletes given username from followers if is available.
     * @param user STRING username that will be deleted from followers list
     * */
    private void deleteUserFromFollowers(String user){
        int control = 0;
        int counter = 0;
        // delete from follower list
        for(String username : getFollowers()){
            if(username == null) break;
            if(username == user){
                control--;
            }else{
                followers[control] = followers[counter];
            }
            control++;
            counter++;
        }if(control == 0) followers[control] = null;
    }
    /**
     * deleteUserFromFollowing function deletes given username from following if is available.
     * @param user STRING username that will be deleted from following list
     * */
    private void deleteUserFromFollowing(String user){
        int control = 0;
        int counter = 0;
        // delete from following list
        for(String username : getFollowing()){
            if(username == null) break;
            if(username == user){
                control--;
            }else{
                following[control] = following[counter];
            }
            control++;
            counter++;
        }if(control == 0) following[control] = null;
    }
        /* end of block functions field */

        /* view functions */
    /**
     * viewProfile function views profile which is defined as parameter of viewedProfileUsername
     * also, prints all necessary information of viewed profile.
     * @param viewedProfileUsername STRING username that will be viewed
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * */
    public void viewProfile(Account [] accounts, String viewedProfileUsername){
        boolean logStatus = isLogin();

        if(getUsername() == viewedProfileUsername) {System.out.println("You are viewing own profile!");}

        for(Account account : accounts){
            if(account.getUsername() == viewedProfileUsername){
                if( account.isBlocked(getUsername()) ){ // checking of viewer user is blocked or not
                    System.out.println("You cannot view this page because you are blocked!");
                    return;
                }
                int followingCount = account.getFollowingCount();
                int followersCount = account.getFollowersCount();
                System.out.println("User ID: " + account.getAccountId());
                System.out.println("Username : " + account.getUsername());
                System.out.println("Location: " + account.getLocation());
                System.out.println("Birth Date: " + account.getBirthdate());
                System.out.println(account.getUsername() + " is following " + followingCount + " account(s) and has " + followersCount + " follower(s).");

                if(followersCount != 0){
                    System.out.print("The followers of " +account.getUsername() + " are: ");
                    for(String username : account.getFollowers() ) {
                        if(username == null) break;
                        System.out.print(username +", ");
                    }System.out.println();
                }

                if(followingCount != 0){
                    System.out.print(account.getUsername() + " is following ");
                    for(String username : account.getFollowing() ) {
                        if(username == null) break;
                        System.out.print(username +", ");
                    }System.out.println();
                }

                System.out.println(account.getUsername() + " has " + account.getPostCount() + " post(s).");
                currentStatus = viewedProfileUsername;
                break;
            }
        }
        System.out.println();
        if(!logStatus)currentStatus = "logout";
    }
    /**
     * viewPosts function views all posts of user which is defined as parameter of viewedProfileUsername
     * also, prints all necessary information of posts.
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * */
    public void viewPosts(Account [] accounts){
        if(!isLogin()) {
            System.out.println("You have to login!");
            return;
        }
        boolean flag = true;
        for(Account account : accounts){
            if (account.getUsername() == currentStatus) {

                for (Post post : account.getPosts()){
                    if(post==null) return;
                    System.out.println("( PostID: " + post.getPostID() + ") " + idtoUsername(accounts, post.getAccountID()) + ": " + post.getContent());
                    flag = false;
                }
                break;
            }
        }
        if(flag) System.out.println("There is no posts!");
    }
    /**
     * viewPostsInteractions function views all post interactions of user which is defined as parameter of viewedProfileUsername
     * also, prints all necessary information of posts and its interactions.
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * */
    public void viewPostsInteractions(Account [] accounts){
        if(!isLogin()) {
            System.out.println("You have to login!");
            return;
        }
        for(Account account : accounts){
            if (account.getUsername() == currentStatus){

                for(Post post : account.getPosts()) {
                    if(post == null) break;
                    System.out.println("-------------");
                    System.out.println("( PostID: " + post.getPostID() + ") " + idtoUsername(accounts, post.getAccountID()) + ": " + post.getContent());
                    post.showInteractions(accounts);
                }
                break;
            }
        }
    }
        /* end of view functions*/
    /**
     * follow function follows account which is defined with username which is given as a parameter
     * also, followed account's followers list updated.
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * @param username STRING following account username
     * */
    public void follow(Account [] accounts, String username){
        if (!isLogin()) {
            System.out.println("You have to login!");
            return;
        }
        if(isFollowing(username)) {
            System.out.println("You have currently following this account!");return;}
        for(Account account : accounts){
            if(account.username == username){
                account.addFollowers(getUsername());
                following[ getFollowingCount() ] = username;
                break;
            }
        }
    }

    /**
     * isFollowing function control of given username is following or not
     * @param username STRING following account username
     * @return BOOLEAN given username is following or not
     * */
    private boolean isFollowing(String username){
        for(String tempUsername : getFollowing()){
            if(tempUsername == null) return false;
            if(tempUsername == username) return true;
        }
        return false;
    }
    /**
     * addFollowers function add followers to followers list
     * @param username STRING follower account username
     * */
    private void addFollowers(String username) {followers[ getFollowersCount() ] = username;}
    /**
     * getFollowingCount function return number of following account
     * @return INT number of following account
     * */
    private int getFollowingCount() {
        int counter = 0;

        for(String username : getFollowing()){
            if(username == null)
                return counter;
            counter++;
        }
        return counter;
    }
    /**
     * getFollowersCount function return number of getFollowersCount account
     * @return INT number of followers account
     * */
    private int getFollowersCount() {
        int counter = 0;

        for(String username : getFollowers()){
            if(username == null)
                return counter;
            counter++;
        }
        return counter;
    }

    /* end of profile field */

    /* message field */
    /**
     * sendMessage function sends message to target account
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * @param receiverUsername STRING target account
     * @param content STRING content of message
     * */
    public void sendMessage(Account [] accounts,String receiverUsername, String content){
        if ( !isLogin() ) {
            System.out.println("You have to login!");
            return; // user not login, can not send any message
        }

        boolean flag = false;
        // controlling of sender account is following or not
        for(String username : getFollowing()) {
            if (username == null) break;
            else if (username == receiverUsername) flag = true;
        }
        if(!flag){
            System.out.println("You are not following " + receiverUsername + ", please follow it or maybe s/he has blocked you!");
            return;
        }

        int mID = 0;

        // calculating of message id
        for(Account account : accounts)
            mID += account.getMessageCount();

        Account receiver = null;

        for(Account account : accounts){
            if(account == null)
                break;
            else if(account.getUsername() == receiverUsername) { // controlling of username
                if( account.isBlocked(getUsername()) ){ // controlling of sender account is blocked or not
                    System.out.println("You cannot send message this user because you are blocked!");
                    return;
                }
                receiver = account;
            }
        }

        if(receiver == null) return;

        messages[ getMessageCount() ] = new Message(mID, getAccountId(), receiver.getAccountId(), content);
        receiver.getMessage(this, content, mID);
    }
    /**
     * getMessage function gets message to sender account
     * @param sender ACCOUNT which sends message
     * @param mID INT unique id of message
     * @param content STRING content of message
     * */
    public void getMessage(Account sender, String content, int mID){
        messages[ getMessageCount() ] = new Message(mID, sender.getAccountId(), getAccountId(), content);
    }
    /**
     * showInbox function prints inbox to screen
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * */
    public void showInbox(Account [] accounts){
        if ( !isLogin() ) {
            System.out.println("You have to login!");
            return;
        }
        if( getMessageCount() == 0) return;
        System.out.println("--------------------------");
        System.out.println("Viewing inbox...");
        for(Message message : getMessages()){
            if (message == null) break;
            else{
                if(message.getReceiverID() == getAccountId()){
                    System.out.println("Message ID: " + message.getMessageID());
                    System.out.println("From: " + idtoUsername(accounts, message.getSenderID()));
                    System.out.println("To: "+ idtoUsername(accounts, message.getReceiverID()));
                    System.out.println("Message: "+message.getContent());
                    System.out.println("-----");
                }
            }
        }
        System.out.println("--------------------------");
    }

    /**
     * overloaded function.
     * showInbox function prints inbox to screen but specific username
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * @param username STRING get message from specific username
     * */
    public void showInbox(Account [] accounts, String username){
        if ( !isLogin() ) {
            System.out.println("You have to login!");
            return;
        }
        System.out.println("--------------------------");
        System.out.println("Viewing inbox...");
        for(Message message : getMessages()){
            if (message == null) break;
            else{
                if(message.getReceiverID() == getAccountId() && idtoUsername(accounts, message.getSenderID()) == username){
                    System.out.println("Message ID: " + message.getMessageID());
                    System.out.println("From: " + idtoUsername(accounts, message.getSenderID()));
                    System.out.println("To: "+ idtoUsername(accounts, message.getReceiverID()));
                    System.out.println("Message: "+message.getContent());
                    System.out.println("-----");
                }
            }
        }
        System.out.println("--------------------------");
    }
    /**
     * showOutbox function prints outbox to screen
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * */
    public void showOutbox(Account [] accounts){
        if ( !isLogin() ) {
            System.out.println("You have to login!");
            return;
        }

        System.out.println("--------------------------");
        System.out.println("Viewing outbox...");
        for(Message message : getMessages()){
            if (message == null)
                break;
            else{
                if(message.getSenderID() == getAccountId()){
                    System.out.println("Message ID: " + message.getMessageID());
                    System.out.println("From: " + idtoUsername(accounts, message.getSenderID()));
                    System.out.println("To: "+idtoUsername(accounts, message.getReceiverID()));
                    System.out.println("Message: "+message.getContent());
                    System.out.println("------");
                }
            }
        }
        System.out.println("--------------------------");
    }
    /**
     * showCountOutbox function prints count of message in outbox.
     * */
    public void showCountOutbox(){
        System.out.println("There is/are " + getCountOutbox() + " int the outbox.");
    }
    /**
     * showCountInbox function prints count of message in inbox.
     * */
    public void showCountInbox(){
        System.out.println("There is/are " + getCountInbox() + " int the inbox.");
    }
    /**
     * getCountOutbox function count number of messages in outbox.
     * @return INT number of messages in outbox.
     * */
    private int getCountOutbox(){
        int counter = 0;
        for(Message message : getMessages()){
            if (message == null) break;

            else
            if(message.getSenderID() == getAccountId())
                counter++;

        }
        return counter;
    }
    /**
     * getCountOutbox function count number of messages in inbox.
     * @return INT number of messages in outbox.
     * */
    private int getCountInbox(){
        int counter = 0;
        for(Message message : getMessages()){
            if (message == null) break;

            else
                if(message.getReceiverID() == getAccountId())
                    counter++;

        }
        return counter;
    }
    /**
     * getCountOutbox function count number of messages in outbox + inbox.
     * @return INT number of messages in outbox.
     * */
    private int getMessageCount(){
        int counter = 0;

        for(Message message : messages){
            if(message == null || message.getMessageID() == -1)
                return counter;

            counter++;
        }

        return counter;
    }
    /**
     * idtoUsername function try to find username by using id
     * @param accounts ACCOUNT ARRAY which holds all created account objects
     * @param id INT value of target id
     * @return STRING username of id*/
    private String idtoUsername(Account[] accounts, int id) {
        for (Account account : accounts) {
            if (account == null)
                break;
            else if(account.getAccountId() == id){
                return account.getUsername();
            }
        }
        return "null";
    }
    /* end of message field */

    /* post functions */
    /**
     * postPost function posts a post with content
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * @param content STRING content of post
     * */
    public void sharePost(Account [] accounts, String content){
        if ( !isLogin() ) {
            System.out.println("You have to login!");
            return; // user not login, can not post any post
        }
        int id = 0;

        for (Account account : accounts)
            id += account.getPostCount();

        posts[ getPostCount() ] = new Post(id, accountId, content);
    }
    /**
     * likePost function like a post, but it can only like current viewed account post
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * @param postID ID content of post
     * */
    public void likePost(Account [] accounts, int postID){
        if (!isLogin()) {
            System.out.println("You have to login!");
            return;
        }
        int likeCount = 0;

        // calculating of unique like id
        for (Account account : accounts) {
            for (Post post : account.getPosts()) {
                if (post == null) break;
                likeCount += post.getLikeCount();
            }
        }

        for(Account account : accounts){
            if( account.getUsername() == currentStatus){ // checking of active account in user page or not
                for(Post post : account.getPosts()){
                    if(post == null) {System.out.println("There is no post as a given id.");return;} // there is no post with given id
                    if(post.getPostID() == postID){
                        post.addLike(likeCount, getAccountId());
                        break;
                    }
                }
                break;
            }
        }

    }
    /**
     * likePost function comment a post, but it can only comment current viewed account post
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * @param postID ID content of post
     * @param content STRING content of comment
     * */
    public void commentPost(Account [] accounts, int postID, String content){
        if (!isLogin()) {
            System.out.println("You have to login!");
            return;
        }
        int commentCount = 0;
        boolean flag = true;
        for (Account account : accounts) {
            for (Post post : account.getPosts()) {
                if (post == null) break;
                commentCount += post.getCommentCount();
            }
        }

        for(Account account : accounts){
            if( account.getUsername() == currentStatus){ // checking of active account in user page or not
                for(Post post : account.getPosts()){
                    if (post == null) break; // there is no post with given id

                    if(post.getPostID() == postID){
                        post.addComment(commentCount, getAccountId(), content);
                        flag = false;
                        break;
                    }
                }
                break;
            }
        }
        if(flag) System.out.println("You have to view the user page, and give correct id!");
    }
    /**
     * getPostCount function counts all posts
     * @return INT number of post */
    private int getPostCount(){
        int counter = 0;

        for(Post post : posts){
            if(post == null || post.getPostID() == -1)
                return counter;

            counter++;
        }

        return counter;
    }

    /* end of post functions */

    /* helper function */
    /**
     * login function login account if another account does not log in.
     * @param accounts ACCOUNT ARRAY which holds all created account objects.
     * */
    public void login(Account [] accounts){
        if(isLogin()) System.out.println("You are already Log in!");
        // if user not logout cannot log in to another account
        for(Account account : accounts)
            if (account.isLogin())
                if(account.getUsername() != username)
                    return;

        currentStatus = "login";
    }
    /**
     * logout function log out from current account
     * */
    public void logout(){
        currentStatus = "logout";
    }
    /**
     * isLogin function check object is log in or not
     * @return BOOLEAN log in or not
     * */
    public boolean isLogin(){
        return currentStatus != "logout" ;
    }

    /* end of helper functions */
    public int getAccountId()    {
        return accountId;
    }

    public void setAccountId(int accountId) {
        this.accountId = accountId;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getBirthdate() {
        return birthdate;
    }

    public void setBirthdate(String birthdate) {
        this.birthdate = birthdate;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
    }

    public Post[] getPosts() {
        return posts;
    }

    public void setPosts(Post[] posts) {
        this.posts = posts;
    }

    public Message[] getMessages() {
        return messages;
    }

    public void setMessages(Message[] messages) {
        this.messages = messages;
    }

    public String[] getFollowing() {
        return following;
    }

    public void setFollowing(String[] following) {
        this.following = following;
    }

    public String[] getFollowers() {
        return followers;
    }

    public void setFollowers(String [] followers) {
        this.followers = followers;
    }

    public String[] getBlockedList() {
        return blockedList;
    }

    public void setBlockedList(String[] blockedList) {
        this.blockedList = blockedList;
    }

    public String getCurrentStatus() {
        return currentStatus;
    }

    public void setCurrentStatus(String currentStatus) {
        this.currentStatus = currentStatus;
    }

    @Override
    public String toString() {
        return "ID: " + getAccountId() + "\nUsername: " + getUsername();
    }
}