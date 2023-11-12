package homework1;

/**
 * The homework1.Interaction Class implements interaction object.
 * homework1.Interaction objects holds unique interaction id, account id who interacted, post id which interacted.
 * homework1.Interaction object gives inheritance to 2 class.
 *
 * @author Ahmet / mens1s
 * @version 0.1
 * @since 2023-03-17
 *
 */
public class Interaction {
    /**
     * unique id of each homework1.Interaction object */
    private int interactionID = 0;
    /**
     * unique id of each account object */
    private int accountID = 0;
    /**
     * unique id of each post object */
    private int postID = 0;

    /**
     * Constructor of homework1.Interaction class
     * @param interactionID INT unique id for each like object
     * @param accountID INT id of interacted account
     * @param postID INT id of interacted post*/
    Interaction(int interactionID, int accountID, int postID){
        this.interactionID = interactionID;
        this.accountID = accountID;
        this.postID = postID;
    }

    public int getInteractionID() {
        return interactionID;
    }

    public void setInteractionID(int interactionID) {
        this.interactionID = interactionID;
    }

    public int getAccountID() {
        return accountID;
    }

    public void setAccountID(int accountID) {
        this.accountID = accountID;
    }

    public int getPostID() {
        return postID;
    }

    public void setPostID(int postID) {
        this.postID = postID;
    }
}
