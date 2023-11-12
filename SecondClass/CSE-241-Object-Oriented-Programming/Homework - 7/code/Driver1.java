import java.util.concurrent.TimeUnit;

/**
 * Driver2 Class tests each function and plays automatically
 */
public class Driver1 {
    /**
     * Testing classes
     * @param args none
     */
    public static void main(String[] args) {
        int height = 10;
        int len = 10;
        String tetrominos = "IOTJLSZ";

        System.out.println("Automatic tests are started!");
        System.out.println("Testing Each Tetromino function");

        waitMilliseconds(100);

        for(int idx = 0; idx < tetrominos.length(); idx++){
            Tetromino tet = new Tetromino(Tetromino.tetromino.valueOf(tetrominos.charAt(idx)+""));
            System.out.println("Testing Tetromino : " + tetrominos.charAt(idx)  + " Testing print function : ");
            tet.print();
            waitMilliseconds(50);
            System.out.println("Testing 3 times of rotate function : ");
            for(int a = 0; a < 3 ; a++){
                tet.rotate(true);
                tet.print();
            }
            waitMilliseconds(150);
        }
        System.out.println("End of testing Tetromino classes! canFit function tries now!");

        System.out.println("Tetris game play will be automatically [TESTs - all functions in tetris class] [Tests all tetromino Pieces] ");
        waitMilliseconds(150);
        Tetris.playGameAutomatically(height, len, tetrominos);

        tetrominos = "IOOSTSSTZTS";
        System.out.println("Tetris game play will be automatically [TESTs - all functions in tetris class] [Tests Random tetromino Pieces] ");
        waitMilliseconds(150);
        Tetris.playGameAutomatically(height, len, tetrominos);

        tetrominos = "IOOOOTSLLLZZSLT";
        System.out.println("Tetris game play will be automatically [TESTs - all functions in tetris class] [Tests all tetromino Pieces] ");
        waitMilliseconds(150);
        Tetris.playGameAutomatically(height, len, tetrominos);

        tetrominos = "ITTLSSLZZLS";
        System.out.println("Tetris game play will be automatically [TESTs - all functions in tetris class] [Tests all tetromino Pieces] ");
        waitMilliseconds(150);
        Tetris.playGameAutomatically(height, len, tetrominos);

        tetrominos = "IIITTTTLLLSSSZZZJJJ";
        System.out.println("Tetris game play will be automatically [TESTs - all functions in tetris class] [Tests Same 3 tetromino Pieces] ");
        waitMilliseconds(150);
        Tetris.playGameAutomatically(height, len, tetrominos);

        System.out.println("Automatic test are ended!");
    }

    private static void waitMilliseconds(int ms){
        try {
            TimeUnit.MILLISECONDS.sleep(ms);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
