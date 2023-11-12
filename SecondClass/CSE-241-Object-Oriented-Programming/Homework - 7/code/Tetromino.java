import java.util.ArrayList;
public class Tetromino extends Throwable {
    /**
     * Enum type of tetromino
     */
    public enum tetromino{
        I("I"), O("O"), T("T"), J("J"), L("L"), S("S"), Z("Z");
        private String type;
        tetromino(String type){this.type = type;}
        public String getType(){return this.type;}
    }
    private ArrayList < ArrayList<Character> > shape;
    private ArrayList < ArrayList<Character> > erasedShape;

    /**
     * Constructor of Tetromino
     * @param type Enum type of Tetromino to create and fill shape
     * @throws RuntimeException If it gets unpredictable char as a parameter it throws an exception. but I don't expect it will work maybe some heap problems can occur it.
     */
    public Tetromino(tetromino type){
        shape = new ArrayList<ArrayList<Character>>();

        ArrayList<Integer> locs = new ArrayList<Integer>();
        char c;

        switch (type){
            case I:
                c = 'I';
                locs.add(0);locs.add(8);locs.add(4);locs.add(12);
                break;
            case O:
                c = 'O';
                locs.add(12);locs.add(8);locs.add(13);locs.add(9);
                break;
            case T:
                c = 'T';
                locs.add(8);locs.add(9);locs.add(10);locs.add(13);
                break;
            case J:
                c = 'J';
                locs.add(5);locs.add(9);locs.add(13);locs.add(12);
                break;
            case L:
                c = 'L';
                locs.add(4);locs.add(8);locs.add(12);locs.add(13);
                break;
            case S:
                c = 'S';
                locs.add(10);locs.add(9);locs.add(13);locs.add(12);
                break;
            case Z:
                c = 'Z';
                locs.add(8);locs.add(9);locs.add(13);locs.add(14);
                break;
            default:
                throw new RuntimeException("Wrong Type TETROMINO TYPE!");
        }

        for(int h = 0; h < 4; ++h){
            shape.add(new ArrayList<Character>());

            for(int l = 0; l < 4; ++l)
                shape.get(h).add(' ');

        }

        for(int loc : locs){
            shape.get(loc / 4).set(loc%4, c);
        }
        //locs.forEach( (loc) -> {
         //   shape.get(loc / 4).set(loc%4, c);
        //} );
    }

    /**
     * This function controls if gameArea and erased shape has an instersect point if it is returns false
     * @param gameArea This is rectangle area from game Area, sizes from erased shape
     * @return erased shape addable to game area
     */
    public boolean canFit(ArrayList < ArrayList<Character> > gameArea){
        boolean flag = true;

        for(int h = 0; h < gameArea.size() && flag; ++h)
            for(int l = 0; l < gameArea.get(0).size() && flag; ++l)
                if(gameArea.get(h).get(l) != ' ' && erasedShape.get(h).get(l) != ' ')
                    flag = false;

        return flag;
    }

    /**
     * Rotate to Tetromino
     * @param isLeft boolean value to shape is returns to left or not
     */
    public void rotate(boolean isLeft){
        int rotCount = 0;
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
     * prints Tetromino Pieces
     */
    public void print(){
        for(ArrayList<Character> line : shape){
            for(Character l : line){
                System.out.print(l);
            }
            System.out.println();
        }
        //shape.forEach( line -> {
        //    line.forEach(System.out::print);
        //    System.out.println();
        //});
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
            int maxH = erasedShape.size();

            for(int h = 3; h >= 0; h--)
                if(erasedShape.get(h).get(maxL - 1) != ' ')
                    flag = false;


            if(flag)
                for(int h = 0; h < maxH; h++)
                    erasedShape.get(h).remove(maxL-1);

        }
        // checking left side
        flag = true;
        while(flag){
            int maxH = erasedShape.size();

            for(int h = 3; h >= 0; h--)
                if(erasedShape.get(h).get(0) != ' ')
                    flag = false;

            if(flag)
                for(int h = 0; h < maxH; h++)
                    erasedShape.get(h).remove(0);

        }
        // checking upside
        flag = true;
        while (flag){
            int maxL = erasedShape.get(0).size();

            for(int l = maxL-1; l >= 0; l--)
                if(erasedShape.get(0).get(l) != ' ')
                    flag = false;

            if(flag)
                erasedShape.remove(0);
        }
        // checking downside
        flag = true;
        while (flag){
            int maxL = erasedShape.get(0).size();
            int maxH = erasedShape.size();

            for(int l = maxL-1; l >= 0; l--)
                if(erasedShape.get(maxH-1).get(l) != ' ')
                    flag = false;

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
}
