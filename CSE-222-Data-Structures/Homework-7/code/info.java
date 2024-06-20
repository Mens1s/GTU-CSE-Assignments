import java.util.ArrayList;
/**
 * this class holds words and their count
 * */
public class info {
    /**
     * integer variable that holds word count*/
    private int count;
    /**
     * arraylist variable that holds all words*/
    private final ArrayList<String> words;
    /**
     * constructor of info class it initialize count var to zero and add parameter to arraylist
     * @param word String value that will add to arraylist*/
    public info(String word) {
        count = 0;
        words = new ArrayList<>();
        push(word);
    }
    /**
     * Adds Parameter 'word' to arraylist
     * @param word String value of word which will be added to arraylist*/
    public void push(String word){
        words.add(word);
        count++;
    }
    /**
     * returns count of words
     * @return count of words*/
    public int getCount() {
        return count;
    }
    /**
     * returns list of words
     * @return list of words*/
    public ArrayList<String> getWords() {
        return words;
    }

}
