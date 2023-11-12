/**
 * Provides the tetromino class and enum tetromino type
 */

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
public class Tetromino {

    /**
     * Enum type of tetromino
     */
    public enum tetromino{
        I(), O(), T(), J(), L(), S(), Z();

        tetromino(){
        }

    }

    public ArrayList<ArrayList<Character>> getShape() {
        return shape;
    }
    private final ArrayList < ArrayList<Character> > shape;
    private ArrayList < ArrayList<Character> > erasedShape;
    private Color color;
    /**
     * Constructor of Tetromino
     * @param type Enum type of Tetromino to create and fill shape
     * @throws RuntimeException If it gets unpredictable char as a parameter it throws an exception. but I don't expect it will work maybe some heap problems can occur it.
     */
    public Tetromino(tetromino type){
        shape = new ArrayList<ArrayList<Character>>();

        ArrayList<Integer> locs = new ArrayList<Integer>();
        char c;

        switch (type) {
            case I : {
                c = 'I';
                color = Color.CYAN;
                locs.add(0);
                locs.add(8);
                locs.add(4);
                locs.add(12);
                break;
            }
            case O : {
                c = 'O';
                color = Color.yellow;
                locs.add(12);
                locs.add(8);
                locs.add(13);
                locs.add(9);
                break;
            }
            case T : {
                c = 'T';
                color = Color.MAGENTA;
                locs.add(8);
                locs.add(9);
                locs.add(10);
                locs.add(13);
                break;
            }
            case J : {
                c = 'J';
                color = Color.blue;
                locs.add(5);
                locs.add(9);
                locs.add(13);
                locs.add(12);
                break;
            }
            case L : {
                c = 'L';
                color = Color.orange;
                locs.add(4);
                locs.add(8);
                locs.add(12);
                locs.add(13);
                break;
            }
            case S : {
                c = 'S';
                color = Color.green;
                locs.add(10);
                locs.add(9);
                locs.add(13);
                locs.add(12);
                break;
            }
            case Z : {
                c = 'Z';
                color = Color.red;
                locs.add(8);
                locs.add(9);
                locs.add(13);
                locs.add(14);
                break;
            }
            default : {
                c = ' ';
            }
        }

        for(int h = 0; h < 4; ++h){
            shape.add(new ArrayList<Character>());

            for(int l = 0; l < 4; ++l)
                shape.get(h).add(' ');

        }
        for(int loc : locs){
            shape.get(loc / 4).set(loc%4, c);
        }
        //locs.forEach( (loc) -> shape.get(loc / 4).set(loc%4, c));

    }

    /**
     * Rotate to Tetromino
     * @param isLeft boolean value to shape is returns to left or not
     */
    public void rotate(boolean isLeft){
        int rotCount;
        if(isLeft) rotCount = 1;
        else rotCount = 3;

        for(int rot = 0; rot < rotCount; ++rot ){

            for(int i = 0; i < 2; i++) {
                for(int j = i; j < 3 - i; j++) {
                    char temp = shape.get(i).get(j);
                    shape.get(i).set(j, shape.get(3-j).get(i));
                    shape.get(3-j).set(i, shape.get(3-i).get(3-j));
                    shape.get(3-i).set(3-j, shape.get(j).get(3-i));
                    shape.get(j).set(3-i,temp);
                }
            }

        }
    }

    /**
     * This function deletes unnecessary area in the tetromino
     * @return 2D Char Array Erased shape will be returned which is erased from spaces
     */
    public ArrayList < ArrayList<Character> > getErasedShape(){
        setErasedShape();

        boolean flag = true;
        // checking right side
        while(flag){
            int maxL = erasedShape.get(0).size();

            for(int h = 3; h >= 0; h--)
                if (erasedShape.get(h).get(maxL - 1) != ' ') {
                    flag = false;
                    break;
                }


            if(flag)
                for (ArrayList<Character> characters : erasedShape) characters.remove(maxL - 1);

        }
        // checking left side
        flag = true;
        while(flag){

            for(int h = 3; h >= 0; h--)
                if (erasedShape.get(h).get(0) != ' ') {
                    flag = false;
                    break;
                }

            if(flag)
                for (ArrayList<Character> characters : erasedShape) characters.remove(0);

        }
        // checking upside
        flag = true;
        while (flag){
            int maxL = erasedShape.get(0).size();

            for(int l = maxL-1; l >= 0; l--)
                if (erasedShape.get(0).get(l) != ' ') {
                    flag = false;
                    break;
                }

            if(flag)
                erasedShape.remove(0);
        }
        // checking downside
        flag = true;
        while (flag){
            int maxL = erasedShape.get(0).size();
            int maxH = erasedShape.size();

            for(int l = maxL-1; l >= 0; l--)
                if (erasedShape.get(maxH - 1).get(l) != ' ') {
                    flag = false;
                    break;
                }

            if(flag)
                erasedShape.remove(maxH-1);
        }

        return erasedShape;
    }

    /**
     * This function finds erasedShape sizes
     * @return 1D Integer Array [height, length]
     */
    public ArrayList<Integer> getErasedShapeSizes(){
        getErasedShape();

        ArrayList<Integer> result = new ArrayList<Integer>();

        result.add(erasedShape.size());
        result.add(erasedShape.get(0).size());

        return result;
    }

    /**
     * Creating ErasedShape
     */
    private void setErasedShape(){
        erasedShape = new ArrayList<ArrayList<Character>>();

        for(int h = 0; h < 4; ++h){
            erasedShape.add((new ArrayList<Character>()));
            for(int l = 0; l < 4; ++l){
                erasedShape.get(h).add(shape.get(h).get(l));
            }
        }
        //Collections.copy(erasedShape, shape);
    }

    /**
     * Returns tetromino color
     */
    public Color getColor(){
        return color;
    }

    public boolean canFit(JPanel gameArea, int downMove, int leftMove, int height) {
        boolean flag = true;

        for( int hei = downMove; hei < getErasedShapeSizes().get(0)+downMove && downMove + getErasedShapeSizes().get(0) <= height && flag; hei++ ){
            for( int len = 0 ; len < getErasedShapeSizes().get(1) && flag; len++ ){
                if(gameArea.getComponent(hei * 10 + len + leftMove).getBackground() != Color.lightGray && erasedShape.get(hei-downMove).get(len) != ' ')
                    flag = false;
            }
        }

        return !flag;
    }

}