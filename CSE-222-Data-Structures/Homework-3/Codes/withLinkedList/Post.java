package withLinkedList;
import java.util.LinkedList;

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
    private LinkedList<Like> likes;
    /**
     * comments array holds all comments as a homework1.Comment object */
    private LinkedList<Comment> comments;
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
        likes = new LinkedList<>();
        comments = new LinkedList<>();

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
            if(likes.get(idx).getAccountID() == lAccountID)  // is already liked
                return;

        likes.add(new Like(interactionID, lAccountID, postID));
    }
    /**
     * Count number of account who liked the post.
     * @return number of like count of post*/
    public int getLikeCount() {
        return likes.size();
    }
    /**
     * Print texts, which hold all the username who liked the post, to the screen
     * @param accounts ACCOUNT ARRAY object which holds all created account objects*/

    private void showLikes(LinkedList<Account> accounts) {
        if( getLikeCount() == 0)
            System.out.println("The post has no likes.");
        else {
            System.out.print("The post was liked by the following account(s): ");

            for (Like like : getLikes())
                System.out.print(idtoUsername(accounts, like.getAccountID()) + ", ");

            System.out.println();
        }
    }
    /**
     * unLike function removes like from post
     * @param lAccountID INT value of account id which liked the post
     * */
    public void unLike(int lAccountID){

        for (Like like : likes){
            if(like.getAccountID() == lAccountID) {
                likes.remove(like);
                break;
            }
        }
    }
    /* end of like field */

    /* comment field */
    /**
     * Print content and username, who commented the post, to the screen
     * @param accounts ACCOUNT ARRAY object which holds all created account objects*/

    private void showComments(LinkedList<Account>accounts) {
        if( getCommentCount() == 0)
            System.out.println("The post has no comment.");
        else {
            System.out.println("The post has " + getCommentCount() + " comment(s) ...");
            int counter = 1;
            for (Comment comment : getComments()) {
                System.out.println("Comment " + counter + ": " + idtoUsername(accounts, comment.getAccountID()) + " said '" + comment.getContent() + "' .");
                counter++;
            }
        }
    }
    /**
     * Count number of account who commented the post.
     * @return INT number of comment count*/
    public int getCommentCount() {
        return comments.size();
    }
    /**
     * addComment function adds comment to post
     * @param interactionID INT value which holds unique interaction id
     * @param lAccountID INT value of account id which commented the post
     * @param content STRING text of post content
     * */
    public void addComment(int interactionID, int lAccountID, String content){
        comments.add(new Comment(interactionID, lAccountID, postID, content));
    }

    /**
     * unComment function adds comment to post
     * @param lAccountID INT value of account id which commented the post
     * */
    public void unComment(int lAccountID){
        for (Comment comment : comments){
            if(comment.getAccountID() == lAccountID) {
                comments.remove(comment);
                break;
            }
        }
    }
    /* end of comment field */

    /* helper field */
    /**
     * idtoUsername function try to find username by using id
     * @param accounts ACCOUNT ARRAY which holds all created account objects
     * @param id INT value of target id
     * @return STRING username of id*/
    private String idtoUsername(LinkedList<Account> accounts, int id) {
        for (Account account : accounts) {
            if(account.getAccountId() == id){
                return account.getUsername();
            }
        }
        return "null";
    }
    /**
     * shows interaction's info
     * @param accounts ACCOUNT ARRAY accounts created so far*/
    public void showInteractions(LinkedList<Account> accounts) {
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

    public LinkedList<Like> getLikes() {
        return likes;
    }

    public void setLikes(LinkedList<Like> likes) {
        this.likes = likes;
    }

    public LinkedList<Comment> getComments() {
        return comments;
    }

    public void setComments(LinkedList<Comment> comments) {
        this.comments = comments;
    }

}
