package withLinkedList;

/**
 * The homework1.Comment Class implements comment object which inherited from homework1.Interaction class.
 * homework1.Comment objects holds unique interaction id, account id who liked, post id which liked, content of comment.
 *
 * @author Ahmet / mens1s
 * @version 0.1
 * @since 2023-03-17
 *
 */
public class Comment extends Interaction {
    /**
     * content holds user's comment */
    static int commentCount = 0;
    private String content;
    /**
     * Constructor of homework1.Comment class
     * @param interactionID INT unique id for each comment object
     * @param accountID INT id of comment account
     * @param postID INT id of comment post
     * @param content STRING content of post*/
    Comment(int interactionID, int accountID, int postID, String content) {
        super(interactionID, accountID, postID);
        this.content = content;
        commentCount++;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }
}
