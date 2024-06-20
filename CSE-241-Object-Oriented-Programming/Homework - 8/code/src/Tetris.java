/**
 * Provides the Tetris class
 * Static methods and main method defined in here
 * GUI and most of the logic works in here
 */

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.ArrayList;
import javax.swing.*;
import javax.swing.border.EmptyBorder;

public class Tetris extends JFrame implements KeyListener {
    private final int height = 20;
    private final int length = 10;
    private int leftMove = 0;
    private int score = 0;
    private Tetromino tetromino;
    private long gameSpeed = 150;
    private ArrayList<Integer> sizes;
    private final JPanel gameArea;
    private final JPanel nextPieceShape;
    private final JLabel scoreText;
    private boolean isDeciding = false;
    private static boolean isStart = false;
    private int downMove = 0;
    /**
     * This constructor creates gui and add necessary elements:
     * 1- gameArea JPanel with GridLayout to hold buttons which are used to playing game
     * 2- scorePanel JPanel with GridLayout to hold score and nextPiece label
     * 3- ContPanel JPanel with GridLayout to hold START-PAUSE-RESTART-QUIT buttons
     * etc. also it creates and listen to listeners
     */
    Tetris(){
        super();
        Helper.setLayout();

        setTitle("Tetris Game via Mens1s");
        setSize(500, 750);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocation( Helper.screenCenter("x",getSize()), Helper.screenCenter("y",getSize()) ); // program located mid-point of screen

        // Game Area settings start
        gameArea = new JPanel();
        gameArea.setLayout(new GridLayout(height, length)); // test here

        for (int i = 0; i < 200; i++){
            JButton temp = new JButton();
            temp.setBackground(Color.LIGHT_GRAY);
            gameArea.add(temp);
        }

        gameArea.setBorder(new EmptyBorder(10, 20, 20, 20));
        // Game Area settings end

        // score field settings stars
        JPanel scorePanel = new JPanel();
        scorePanel.setLayout( new GridLayout(6,4) );

        nextPieceShape = new JPanel();
        nextPieceShape.setLayout(new GridLayout(4,4));

        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++){
                JButton temp = new JButton();
                temp.setBackground(Color.LIGHT_GRAY);
                nextPieceShape.add(temp);
            }
        }

        scoreText = new JLabel("Score = " + score);

        scorePanel.add(scoreText);
        scorePanel.add(new JLabel("Next Piece : "));
        scorePanel.add(nextPieceShape);

        scorePanel.setBorder(new EmptyBorder(0, 0, 0, 10));
        // score field settings end

        // control panel settings start
        JPanel contPanel = new JPanel();
        contPanel.setLayout(new GridLayout(1,4) );

        // creating and listening to buttons
        JButton startButton = new JButton("Start");
        startButton.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                downMove = 0;
                score = 0;
                Helper.cleanGameArea(gameArea,height, length);
                if(Helper.confirm("start"))
                    isStart = true;
            }
        });

        startButton.setFocusable(false);

        JButton pauseButton = new JButton("Pause");
        pauseButton.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                isDeciding = true;
                Helper.confirm("pause");
                isDeciding = false;
            }
        });

        pauseButton.setFocusable(false);

        JButton restartButton = new JButton("Restart");
        restartButton.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                isDeciding = true;
                if(Helper.confirm("restart")){
                    downMove = 0;
                    Helper.cleanGameArea(gameArea, height, length);
                }
                isDeciding = false;
            }
        });
        restartButton.setFocusable(false);

        JButton quitButton = new JButton("Quit");
        quitButton.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e){
                isDeciding = true;

                if(Helper.confirm("exit")) {
                    dispose();
                    System.exit(1);
                }

                isDeciding = false;
            }
        });

        quitButton.setFocusable(false);

        contPanel.add(startButton);
        contPanel.add(pauseButton);
        contPanel.add(restartButton);
        contPanel.add(quitButton);

        contPanel.setBorder(new EmptyBorder(10, 20, 0, 20) );
        // control panel settings end

        // add panels to main frame
        add(gameArea, BorderLayout.CENTER);
        add(scorePanel, BorderLayout.EAST);
        add(contPanel, BorderLayout.NORTH);

        setFocusable(true);
        addKeyListener(this);
        setVisible(true);
    }
    /**
     * This function checks all thing and move tetromino to user inputs.
     * 1- right/left arrow move tetromino to right/left one movement if there is a tetromino game over or if there is no place then no movement.
     * 2- up arrow to speed up
     * @param tet Tetromino instant random tetromino
     * @return boolean value if tetromino added or not
     */
    private boolean add(Tetromino tet){
        gameSpeed = 250;
        tetromino = tet;

        boolean isAdded = false;

        boolean isTriggered = false;
        downMove = 0;
        leftMove = length / 2;

        while (!isTriggered){
            int LEFT_MOVE = leftMove;
            sizes = tetromino.getErasedShapeSizes(); // [height, length]

            if(tetromino.canFit(gameArea, downMove, LEFT_MOVE, height))
                isTriggered = true;


            else{
                if(tetromino.canFit(gameArea, downMove + 1, LEFT_MOVE, height))
                    isTriggered = true;

                for( int hei = downMove; hei < sizes.get(0)+downMove && downMove + sizes.get(0) <= height ; hei++ ){
                    for( int len = 0 ; len < sizes.get(1); len++ ){
                        if(tetromino.getErasedShape().get(hei-downMove).get(len) != ' ') {
                            gameArea.getComponent(hei * 10 + len + LEFT_MOVE).setBackground(tetromino.getColor());
                            isAdded = true;
                        }
                    }
                }

                if(downMove + sizes.get(0) == height) isTriggered = true;

                Helper.sleep(gameSpeed);

                for( int hei = downMove; hei < sizes.get(0)+downMove && !isTriggered ; hei++ ) {
                    for (int len = 0; len < sizes.get(1); len++) {
                        gameArea.getComponent(hei * 10 + len + LEFT_MOVE).setBackground(Color.LIGHT_GRAY);
                        isAdded = false;
                    }
                }
                downMove++;
            }
            // waiting to user to give decide
            while(isDeciding){
                System.out.print("");
            }
        }
        checkLines();
        return isAdded;
    }
    /**
     * This function checks all lines if there is full line it calls deleteLine function
     */
    private void checkLines() {
        for(int h = 0; h < height; h++){
            boolean delete = true;
            for(int l = 0; l < length; l++)
                if(gameArea.getComponent(h * 10 + l).getBackground() == Color.lightGray)
                    delete = false;

            if(delete) deleteLine(h);
        }
    }
    /**
     * This function deletes h. line.
     * @param h int h is the line number to delete
     */
    private void deleteLine(int h) {
        if(h != 0){
            // animate
            for(int animate = 0; animate < 3; animate++) {
                for (int l = 0; l < length; l++)
                    gameArea.getComponent(h * 10 + l).setBackground(Color.RED);
                Helper.sleep(100);
                for (int l = 0; l < length; l++)
                    gameArea.getComponent(h * 10 + l).setBackground(Color.blue);
                Helper.sleep(100);
            }
            // delete
            for(int hei = h; hei > 0 ; hei--)
                for(int l = 0; l < length; l++)
                    gameArea.getComponent(hei * 10 + l).setBackground(gameArea.getComponent((hei-1)*10 + l).getBackground());

            for(int l = 0; l < length; l++)
                gameArea.getComponent(l).setBackground(Color.LIGHT_GRAY);
        }
        score += height;
        scoreText.setText("Score : " + score);

    }
    /**
     * Key Listener must function
     */
    @Override
    public void keyTyped(KeyEvent e) {}
    /**
     * Key Listener must function
     */
    @Override
    public void keyPressed(KeyEvent e){

        int key = e.getKeyCode();

        switch (key){
            case KeyEvent.VK_LEFT:{ leftMove--;break;}
            case KeyEvent.VK_RIGHT :{ leftMove++;break;}
            case KeyEvent.VK_UP :{ gameSpeed-=20;break;}
        }

        if(key == KeyEvent.VK_SPACE) {

            if( (sizes.get(1) + downMove  >= height ) )
                tetromino.rotate(true);

            else if (! (sizes.get(0)+ leftMove > length) )
                tetromino.rotate(true);

        }
        if(leftMove > length - sizes.get(1) )
            leftMove = length - sizes.get(1) ;

        else if(leftMove <= 0)
            leftMove = 0;
    }
    /**
     * Key Listener must function
     */
    @Override
    public void keyReleased(KeyEvent e) {}
    /**
     * This function finds next random shape and add it to next piece area in the gui
     * @param tetrominoNext nextTetromino of game
     */
    private void setTetrominoNext(Tetromino tetrominoNext) {
        for( int h = 0; h < 4; h++ )
            for( int l = 0;l < 4; l++ )
                nextPieceShape.getComponent(h * 4 + l).setBackground(Color.LIGHT_GRAY);

        for( int h = 0; h < 4; h++ )
            for( int l = 0;l < 4; l++ )
                if(tetrominoNext.getShape().get(h).get(l) != ' ')
                    nextPieceShape.getComponent(h * 4 + l).setBackground(tetrominoNext.getColor());
    }
    /**
     * This static function createt Tetris object and play game as game ended or user failed.
     */
    public static void playGame(){

        boolean cont = true;
        Tetromino tetrominoKnow = new Tetromino(Helper.getRandomTetromino());
        while(cont){
            boolean flag = true;
            Tetris tetris = new Tetris();

            while(flag){
                System.out.print("");
                if(isStart){
                    Tetromino tetrominoNext = new Tetromino(Helper.getRandomTetromino());
                    tetris.setTetrominoNext(tetrominoNext);
                    if(!tetris.add(tetrominoKnow))
                        flag = false;
                    tetrominoKnow = tetrominoNext;
                }
            }

            if(Helper.confirm("finished")) cont = false;
            else {
                cont = true;
                isStart = false;
                tetris.dispose();
            }
        }
        System.exit(1);
    }

    /**
     * Main function
     */
    public static void main(String[] args) {
        playGame();

    }

}