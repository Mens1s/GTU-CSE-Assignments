package homework1;

/**
 * The homework1.Post Class implements post object.
 * homework1.Post object can be liked, commented, and includes content.
 * Also, can hold who liked or commented itself.
 *
 * @author Ahmet / mens1s
 * @version 0.1
 * @since 2023-03-17
 *
 */

public class Post {
    /**
     * unique id of each homework1.Post object */
    private int postID;
    /**
     * unique id of each account object */
    private int accountID;
    /**
     * likes array holds all likes as a homework1.Like object */
    private Like []likes;
    /**
     * comments array holds all comments as a homework1.Comment object */
    private Comment []comments;
    /**
     * content holds post content */
    private String content;

    /**
     * Constructor of homework1.Post
     * @param postID INT holds unique ID for each post
     * @param accountID INT holds unique ID which connected to an account object
     * @param content STRING holds content of post
     * */
    public Post(int postID, int accountID, String content){
        /* assigning variables */
        likes = new Like[100];
        comments = new Comment[100];

        this.postID = postID;
        this.accountID = accountID;
        this.content = content;
    }

    /* interaction field */

    /* like field */

    /**
    * addLike function adds likes to post
     * if user already liked, s/he can not like again
    * @param interactionID INT value which holds unique interaction id
    * @param lAccountID INT value of account id which liked the post
    * */
    public void addLike(int interactionID, int lAccountID){

        for(int idx = 0; idx < getLikeCount(); idx++)
            if(likes[idx].getAccountID() == lAccountID)  // is already liked
                return;

        likes[ getLikeCount() ] = new Like(interactionID, lAccountID, postID);
    }
    /**
     * Count number of account who liked the post.
     * @return number of like count of post*/
    public int getLikeCount() {
        int counter = 0;

        for (Like like : likes){
            if (like == null)
                return counter;
            counter++;
        }
        return counter;
    }
    /**
     * Print texts, which hold all the username who liked the post, to the screen
     * @param accounts ACCOUNT ARRAY object which holds all created account objects*/

    private void showLikes(Account [] accounts) {
        if( getLikeCount() == 0)
            System.out.println("The post has no likes.");
        else {
            System.out.print("The post was liked by the following account(s): ");

            for (Like like : getLikes()) {
                if (like == null) break;
                System.out.print(idtoUsername(accounts, like.getAccountID()) + ", ");
            }
            System.out.println();
        }
    }

    /* end of like field */

    /* comment field */
    /**
     * Print content and username, who commented the post, to the screen
     * @param accounts ACCOUNT ARRAY object which holds all created account objects*/

    private void showComments(Account [] accounts) {
        if( getCommentCount() == 0)
            System.out.println("The post has no comment.");
        else {
            System.out.println("The post has " + getCommentCount() + " comment(s) ...");
            int counter = 1;
            for (Comment comment : getComments()) {
                if (comment == null) break;
                System.out.println("Comment " + counter + ": " + idtoUsername(accounts, comment.getAccountID()) + " said '" + comment.getContent() + "' .");
                counter++;
            }
        }
    }
    /**
     * Count number of account who commented the post.
     * @return INT number of comment count*/
    public int getCommentCount() {
        int counter = 0;

        for (Comment comment : comments){
            if (comment == null)
                return counter;
            counter++;
        }
        return counter;
    }
    /**
     * addComment function adds comment to post
     * @param interactionID INT value which holds unique interaction id
     * @param lAccountID INT value of account id which commented the post
     * @param content STRING text of post content
     * */
    public void addComment(int interactionID, int lAccountID, String content){

        comments[ getCommentCount() ] = new Comment(interactionID, lAccountID, postID, content);
    }
    /* end of comment field */

    /* helper field */
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
    /**
     * shows interaction's info
     * @param accounts ACCOUNT ARRAY accounts created so far*/
    public void showInteractions(Account[] accounts) {
        showLikes(accounts);
        showComments(accounts);
    }

    /* end of helper field */

    /* end of interaction field*/

    public int getPostID() {
        return postID;
    }

    public void setPostID(int postID) {
        this.postID = postID;
    }

    public int getAccountID() {
        return accountID;
    }

    public void setAccountID(int accountID) {
        this.accountID = accountID;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public Like[] getLikes() {
        return likes;
    }

    public void setLikes(Like[] likes) {
        this.likes = likes;
    }

    public Comment[] getComments() {
        return comments;
    }

    public void setComments(Comment[] comments) {
        this.comments = comments;
    }

}
