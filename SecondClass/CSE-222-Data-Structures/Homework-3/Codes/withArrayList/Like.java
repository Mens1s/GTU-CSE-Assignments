package withArrayList;

/**
 * The homework1.Like Class implements like object which inherited from homework1.Interaction class.
 * homework1.Like objects holds unique interaction id, account id who liked, post id which liked.
 *
 * @author Ahmet / mens1s
 * @version 0.1
 * @since 2023-03-17
 *
 */
public class Like extends Interaction {
    /**
     * unique likeCount*/
    static int likeCount = 0;
    /**
     * Constructor of homework1.Like class
     * @param interactionID INT unique id for each like object
     * @param accountID INT id of liked account
     * @param postID INT id of liked post
     */
    Like(int interactionID, int accountID, int postID) {
        super(interactionID, accountID, postID);
        likeCount++;
    }

}
