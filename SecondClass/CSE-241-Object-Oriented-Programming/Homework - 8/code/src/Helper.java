/**
 * Provides the helper static functions
 */

import javax.swing.*;
import java.awt.*;
import java.util.Random;
import java.util.concurrent.TimeUnit;

public class Helper {
    /**
     * This function checks all layouts and if there is Nimbus layout set as a default layout to better user experience
     */
    public static void setLayout(){
        for(UIManager.LookAndFeelInfo info : UIManager.getInstalledLookAndFeels()){
            if("Nimbus".equals(info.getName())){
                try{
                    UIManager.setLookAndFeel(info.getClassName());
                }catch (Exception e){
                    throw new RuntimeException(e);
                }
                break;
            }
        }
    }
    /**
     * This function finds center point of parameter point
     * @param point String dimensionText
     * @param size Dimension
     * @return int center point of param => Point
     */
    public static int screenCenter(String point, Dimension size){
        if(point.equals("x"))
            return (Toolkit.getDefaultToolkit().getScreenSize().width - size.width) / 2;
        else if( point.equals("y") )
            return (Toolkit.getDefaultToolkit().getScreenSize().height - size.height) / 2;
        return 0;
    }
    /**
     * This function selects randomly letter of tetrominos
     * @return tetromino
     */
    public static Tetromino.tetromino getRandomTetromino(){
        char [] arr = {'I', 'O', 'T', 'J', 'L', 'S', 'Z'};
        Random random = new Random();
        int select = random.nextInt(arr.length);
        return Tetromino.tetromino.valueOf( arr[select] + "" );
    }

    /**
     * This function waits as long as gameSpeed parameter
     * @param time Long parameter of wait type
     */
    public static void sleep(long time) {
        try {
            TimeUnit.MILLISECONDS.sleep(time);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
    /**
     * This function decides to message and give OptionPane to user decide
     * @param str String message
     * @return boolean value if user accept it or not
     */
    public static boolean confirm(String str){
        String msg = "";
        if(str.equals("exit")){msg = "Are you sure to exit?";}
        else if(str.equals("restart")){msg = "Are you sure to restart?";}
        else if(str.equals("pause")){msg = "Click any button to continue..";}
        else if(str.equals("start")){msg = "Are you want to start game?";}
        else if(str.equals("finished")){msg = "You are defeated. Do you want to exit?";}

        /*switch (str) {
            case "exit" :{msg = "Are you sure to exit?";break;}
            case "restart" :{msg = "Are you sure to restart?";break;}
            case "pause" :{msg = "Click any button to continue..";break;}
            case "start" :{msg = "Are you want to start game?";break;}
            case "finished" :{msg = "You are defeated. Do you want to exit?";break;}
            default :{msg = str;break;}
        };*/
        return JOptionPane.showConfirmDialog(null , msg , "Confirm ", JOptionPane.YES_NO_OPTION) == 0;
    }
    /**
     * This function cleans game area when user is restarting game.
     * @param gameArea JPanel game area
     * @param hei int height of game area
     * @param len int length of game area
     */
    public static void cleanGameArea(JPanel gameArea, int hei ,int len){
        for(int h = hei - 1; h >= 0; h--)
            for(int l = len - 1; l >= 0; l--)
                gameArea.getComponent(10 * h + l ).setBackground(Color.LIGHT_GRAY);
    }

}
