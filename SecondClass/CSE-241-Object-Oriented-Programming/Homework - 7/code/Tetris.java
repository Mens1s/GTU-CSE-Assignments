import java.io.IOException;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.Random;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;

public class Tetris {
    static Scanner inp = new Scanner(System.in);;
    private int height;
    private int len;
    private ArrayList <ArrayList<Character>> gameArea;

     /**
     * This constructor creates game area
     * @author Mens1s
     * @param height It is a height variable of game area
     * @param len It is a length variable of game area
     */
    public Tetris(int height, int len){
        gameArea = new ArrayList<ArrayList<Character>>();
        this.height = height;
        this.len = len;

        for(int h = 0; h < height; ++h){
            gameArea.add(new ArrayList<Character>());
            for(int l = 0; l < len; ++l){
                gameArea.get(h).add(' ');
            }
        }
    }

    /**
     * FindBestMove functions tries to find userPiece can add to game area
     * if it is addable which location best for userPiece
     * @param userPiece User input tetromino piece
     * @return boolean value
     */
    private boolean findBestMove(Tetromino userPiece){

        int rotCount = 0;

        int[] availableMovement = new int[]{-1, -1, -1, -1}; // 3*(downMovement + sizes [0]) + triggered elements at bottom + triggered elements at right, rotCount, rightMovement, downMovement


        for(; rotCount < 3; rotCount++){ // test for three rotation
            ArrayList<Integer> sizes = userPiece.getErasedShapeSizes(); // sizes [0] => height sizes [1] => length
            for(int rightMove = 0; rightMove <= len - sizes.get(1); rightMove++ ){ // test for right movement

                for(int downMovement = 0; downMovement <= height - sizes.get(0); downMovement++){ // test for down movement
                    ArrayList <ArrayList<Character>> possibleGameArea = getPossibleGameArea( rightMove, rightMove+sizes.get(1), downMovement, downMovement+sizes.get(0) );
                    if(userPiece.canFit(possibleGameArea)){

                        int triggeredElementsAtBottomLength = 0;

                        for(int idx = 0; idx < sizes.get(1); idx++)
                            if(userPiece.getErasedShape().get(sizes.get(0) - 1).get(idx) != ' ')
                                triggeredElementsAtBottomLength++;

                        int triggeredElementsAtRightLength = 0;

                        for(int idx = 0; idx < sizes.get(0) && rightMove == len - sizes.get(1); idx++)
                            if(userPiece.getErasedShape().get(idx).get(sizes.get(1) - 1) != ' ')
                                triggeredElementsAtRightLength++;

                        int score = 3*(downMovement + sizes.get(0)) + triggeredElementsAtBottomLength + triggeredElementsAtRightLength;
                        if ( score > availableMovement[0] ){
                            availableMovement[0] = score;
                            availableMovement[1] = rotCount;
                            availableMovement[2] = rightMove;
                            availableMovement[3] = downMovement;
                        }

                    }
                    else    break;


                }
            }
            userPiece.rotate(true);
        }

        if(availableMovement[0] == -1)
            return false;

        // preparing the userTetromino to best fit
        for(int rot = 0; rot < availableMovement[1] + 1 ; rot++)    userPiece.rotate(true);

        addBestPieceToGameAreaAnimate(userPiece, availableMovement);

        return true;
    }

    /**
     * This function gets user tetromino and move it to the best location
     * after movement add to game area
     * @param userPiece User input tetromino piece
     * @param availableMovement Best movement information [score, rotation Count, right Move Count, Down Move Count]
     */
    private void addBestPieceToGameAreaAnimate(Tetromino userPiece, int[] availableMovement) {

        ArrayList<Integer> sizes = userPiece.getErasedShapeSizes(); // sizes [0] => height sizes [1] => length
        String star = new String(new char[len+2]).replace('\0', '*');
        // mid to up loc animating
        int mid = len / 2;
        int control = 1;

        if (availableMovement[2] < mid) control *= -1;
        for(;mid != availableMovement[2]; mid += control) {
            System.out.println(star);
            for (int h = 0; h < height; h++) {
                System.out.print("*");
                for (int l = 0; l < len; l++) {
                    if(l >= mid && l < mid + sizes.get(1) && h < sizes.get(0)){
                        char c = userPiece.getErasedShape().get(h).get(l-mid);
                        if(c == ' ')
                            System.out.print(gameArea.get(h).get(l));
                        else
                            System.out.print(c);
                    }
                    else
                        System.out.print(gameArea.get(h).get(l));
                }
                System.out.println("*");
            }
            System.out.println(star);
            try {
                TimeUnit.MILLISECONDS.sleep(50);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }

            System.out.print("\033[H\033[2J");
            System.out.flush();

        }
        // up to down animating
        for(int go = 0 ; go <= availableMovement[3]; go++){
            System.out.println(star);
            for(int y = 0; y < height ; y++ ){
                System.out.print("*");
                for(int x = 0 ; x < len ; x++){
                    if(y >= go && y < go + userPiece.getErasedShapeSizes().get(0)){
                        if(x >= availableMovement[2] && x < availableMovement[2]+userPiece.getErasedShapeSizes().get(1)){
                            char c = userPiece.getErasedShape().get(y-go).get(x-availableMovement[2]);
                            if(c == ' ')
                                System.out.print(gameArea.get(y).get(x));

                            else
                                System.out.print(c);
                        }
                        else{
                            System.out.print(gameArea.get(y).get(x));
                        }
                    }
                    else{
                        System.out.print(gameArea.get(y).get(x));
                    }
                }System.out.println("*");
            }
            System.out.println(star);
            try {
                TimeUnit.MILLISECONDS.sleep(50);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }

            System.out.print("\033[H\033[2J");
            System.out.flush();
        }
        // adding to game area in here
        for(int h = availableMovement[3]; h < availableMovement[3]+sizes.get(0); h++)
            for(int l = availableMovement[2]; l <  availableMovement[2]+sizes.get(1); l++)
                if(gameArea.get(h).get(l) == ' ')
                    gameArea.get(h).set(l,userPiece.getErasedShape().get(h-availableMovement[3]).get(l-availableMovement[2]));

    }

    /**
     * Aim is to find parameter described rectangle area
     * @param lStart start point of vertical
     * @param lEnd end point of vertical
     * @param hStart start point of horizontal
     * @param hEnd end point of horizontal
     * @return 2D Char Array which includes area which is between in parameters
     */
    private ArrayList<ArrayList<Character>> getPossibleGameArea(int lStart, int lEnd, int hStart, int hEnd) {
        ArrayList<ArrayList<Character>> possibleGameArea = new ArrayList<ArrayList<Character>>();

        for(int h = hStart; h < hEnd; h++){
            possibleGameArea.add(new ArrayList<Character>());

            for(int l = lStart; l < lEnd; l++)
                possibleGameArea.get(h-hStart).add(gameArea.get(h).get(l));

        }

        return possibleGameArea;
    }

    /**
     * Addes to game area User Tetromino if it is possible
     * @param userPiece user input tetromino
     * @return user piece is addable or not
     */
    private boolean add(Tetromino userPiece){
        if(findBestMove(userPiece)) return true;
        else return false;

    }

    /**
     * Prints game area
     */
    private void print(){
        String star = new String(new char[len+2]).replace('\0', '*');
        System.out.println(star);
        for(int h = 0; h < height; h++){

            System.out.print("*");

            for(int l = 0; l < len; l++)
                System.out.print(gameArea.get(h).get(l));

            System.out.println("*");
        }

        System.out.println(star);
        /*
        gameArea.forEach( line -> {
                line.forEach(System.out::print);
                System.out.println();
            }
        );
        */
    }

    /**
     * Gets positive number from user
     * @return integer user input
     */
    private static int getPozInt(){
        boolean flag = true;
        int temp = -1;

        // controlling user enter only numbers
        while(flag){
            try{
                temp = inp.nextInt();
                if( temp > 0 ) flag = false;
                else System.out.println("Enter only positive number : ");
            }
            catch (InputMismatchException e){
                inp.nextLine();
                System.out.print("Please enter number only! : ");
            }
        }

        return temp;
    }

    /**
     * Gets [I-O-T-J-L-S-Z-R] from user
     * @return char from user
     */
    private static char getTetrominoChar(){
        boolean flag = true;
        String temp = "1";

        ArrayList<Character> tetList = new ArrayList<Character>(){ { add('I');add('O');add('T'); add('J'); add('L'); add('S');add('Z');add('R');}};
        // controlling user enter only numbers
        while(flag){
            try{
                temp = inp.next();
                if(temp.length() == 1 && tetList.contains(temp.charAt(0)))
                    flag = false;
            }
            catch (InputMismatchException e){
                inp.nextLine();
                System.out.print("Please enter number only! : ");
            }
            if(flag)
                System.out.print("Enter only one character and [I-O-T-J-L-S-Z] : ");
        }

        return temp.charAt(0);
    }

    /**
     * This function starts game and create Tetris object.
     */
    public static void playGame(){

        int h, l;

        System.out.print("Enter height of game area : ");
        h = getPozInt();

        System.out.print("Enter length of game area : ");
        l = getPozInt();

        Tetris game = new Tetris(h, l);
        char user_tetromino = 'a';

        while(!(user_tetromino == 'q' || user_tetromino == 'Q')){
            System.out.print("Enter tetromino [I-O-T-J-L-S-Z] or R for Random : ");

            user_tetromino = getTetrominoChar();
            
            if(user_tetromino == 'R'){
                char [] arr = {'I', 'O', 'T', 'J', 'L', 'S', 'Z'};
                Random random = new Random();
                int select = random.nextInt(arr.length);
                user_tetromino = arr[select];
            }

            Tetromino tet = new Tetromino(Tetromino.tetromino.valueOf( user_tetromino + "" ));

            if(game.add(tet)) game.print();
            else {System.out.println("You cannot add this item to game area! Your game has been finished!"); user_tetromino='q';}

        }

    }

    /**
     * This function plays Tetris game automatically
     * @param hei height of game Area
     * @param len length of game Area
     * @param tetrominos String which includes tetromino types
     */
    public static void playGameAutomatically(int hei, int len, String tetrominos){
        Tetris game = new Tetris(hei, len);

        for( int i = 0; i < tetrominos.length(); i++)
            game.add( new Tetromino(Tetromino.tetromino.valueOf(tetrominos.charAt(i)+"") ) );

    }
}