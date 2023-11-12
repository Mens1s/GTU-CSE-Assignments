package withLinkedList;

/**
 * The Message Class implements message object.
 * homework1.Message objects holds content, senderID, receiverID and unique messageID
 *
 * @author Ahmet / mens1s
 * @version 0.1
 * @since 2023-03-17
 *
 */
public class Message {
    /**
     * unique id of each homework1.Message object */
    private int messageID;
    /**
     * unique id of each sender account object */
    private int senderID;
    /**
     * unique id of each receiver account object */
    private int receiverID;
    /**
     * content holds sender's message */
    private String content;

    /**
     * Constructor of homework1.Message
     * @param messageID INT holds unique id for each message
     * @param senderID INT holds sender account id
     * @param receiverID INT holds receiver account id
     * @param content STRING holds message content*/
    public Message(int messageID, int senderID, int receiverID, String content){
        this.messageID = messageID;
        this.senderID = senderID;
        this.receiverID = receiverID;
        this.content = content;
    }

    public int getMessageID() {
        return messageID;
    }

    public void setMessageID(int messageID) {
        this.messageID = messageID;
    }

    public int getSenderID() {
        return senderID;
    }

    public void setSenderID(int senderID) {
        this.senderID = senderID;
    }

    public int getReceiverID() {
        return receiverID;
    }

    public void setReceiverID(int receiverID) {
        this.receiverID = receiverID;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }
}
