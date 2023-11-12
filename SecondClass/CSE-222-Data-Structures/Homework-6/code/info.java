import java.util.ArrayList;

public class info {
    private int count;
    private ArrayList<String> words;

    public info(String word) {
        count = 0;
        words = new ArrayList<>();
        push(word);
    }
    public void push(String word){
        words.add(word);
        count++;
    }

    public int getCount() {
        return count;
    }

    public ArrayList<String> getWords() {
        return words;
    }

}
