import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.io.File;
import java.util.Scanner;
import java.util.List;
/**
 * CSE222Map Class that represents txt file as a map
 */
public class CSE222Map {
    /**
     * X_SIZE of Map
    */
    private final int X_SIZE;
    /**
     * Y_SIZE of Map
    */
    private final int Y_SIZE;
    /**
     * filename of Map
    */
    private final String filename;
    /**
     * variable that hold map
    */
    private final ArrayList<ArrayList<Integer>> map;
    /**
     * Start point of Map for X
    */
    private int start_x;
    /**
     * Start point of Map for Y
    */
    private int start_y;
    /**
     * End point of Map for X
    */
    private int end_x;
    /**
     * End point of Map for Y
    */
    private int end_y;
    /**
     * image variable for save
    */
    private BufferedImage image;
    /**
     * Constructor for CSE222Map function
     * @param filename var that holds filename
     * @param X_SIZE var that holds X_SIZE
     * @param Y_SIZE var that hold Y_SIZE
    */
    public CSE222Map(String filename, int X_SIZE, int Y_SIZE){
        this.filename = filename;
        this.X_SIZE = X_SIZE;
        this.Y_SIZE = Y_SIZE;
        map = new ArrayList<>();
        
        // allocoate memory
        for(int i = 0; i < Y_SIZE; i++)
            map.add(new ArrayList<>());

        readMap();
    }
    /**
     * Read map from FileName variable
     */
    private void readMap(){
        // Read the map from the file
        try {
            File myFile = new File(filename);
            Scanner myReader = new Scanner(myFile);
            int i = 0;
            // reading first line to get start point
            String data = myReader.nextLine();
            String[] cordinates = data.split(",");

            start_x = Integer.parseInt(cordinates[1]);
            start_y = Integer.parseInt(cordinates[0]);

            // reading second line to get end point
            data = myReader.nextLine();

            cordinates = data.split(",");
            end_x = Integer.parseInt(cordinates[1]);
            end_y = Integer.parseInt(cordinates[0]);

            // reading rest of the lines to get map
            while(myReader.hasNextLine()) {
                data = myReader.nextLine();
                cordinates = data.split(",");
                for(String s : cordinates){
                    int tempValue = Integer.parseInt(s);
                    if (tempValue == -1) tempValue = 1;

                    map.get(i).add(tempValue);
                    //value ++;
                    //if(value == X_SIZE){
                    //    value = 0;
                    //    i++;
                    //}
                }
                i++;
            }

            myReader.close();
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
    }
    /**
     * Function that converst map to png file
     */
    public void convertPNG(){
        // Convert the map to a PNG file
        image = new BufferedImage(X_SIZE,Y_SIZE+40,BufferedImage.TYPE_INT_RGB);

        for(int y = 0; y < Y_SIZE; y++) {
            for(int x = 0; x < X_SIZE; x++) {
                int value = map.get(y).get(x);
                Color color = (value==0) ? Color.BLACK : Color.WHITE;
                image.setRGB(x,y,color.getRGB());
            }
        }
    }
    /**
     * The function draws line to image. It works only Green and Red.
     * @param path that holds all way edges as list
     * @param color variable that holds line color
     */
    public void drawLine(List path, Color color){
        int bfsLen = -1;
        int djkLen = -1;
        // Draw the path on the map
        ArrayList<ArrayList<Integer>> pathMap = new ArrayList<>(map);

        for(Object point : path) {
            int x = ((CSE222Graph.Point)point).getX();
            int y = ((CSE222Graph.Point)point).getY();
            if(color == Color.RED){
                pathMap.get(y).set(x,2);
                bfsLen = path.size();
            }
            else if(color == Color.GREEN){
                pathMap.get(y).set(x,3);
                djkLen = path.size();
            }
        }
        // creating image graphics
        Graphics2D graphics = image.createGraphics();
        // fill and draw the line
        for(int y = 0; y < Y_SIZE; y++) {
            for ( int x = 0; x < X_SIZE; x++) {
                int value = pathMap.get(y).get(x);

                if(value == 2) {
                    graphics.setColor(Color.RED);
                    graphics.drawLine(x,y,x,y);
                }
                else if(value == 3) {
                    graphics.setColor(Color.GREEN);
                    graphics.drawLine(x,y,x,y);
                }
            }
        }
        // add some information of path to image 
        graphics.setColor(Color.YELLOW);
        graphics.setFont(new Font("Arial", Font.PLAIN, 12));

        int infoX = 10;
        int infoY = Y_SIZE + 10;
        if(bfsLen == -1){
            graphics.drawString("Green Line : Dijkstra, Path Length : "+(djkLen-1) , infoX, infoY+23);
        }else if(djkLen == -1){
            graphics.drawString("Red Line : BFS, Path Length : "+(bfsLen-1), infoX, infoY+3);
        }
        // save in the outputs folder
        try {
            String folderName = "outputs";
            String tempFilename = "";
            // Get the current directory
            String currentDirectory = System.getProperty("user.dir");
            // Create a File object representing the new folder
            File newFolder = new File(currentDirectory + File.separator + folderName);

            // Create the new folder
            if(newFolder.exists()){
                tempFilename += "outputs/";
            }
            // Create the new folder
            else if(newFolder.mkdir()) {
                tempFilename += "outputs/";

            }

            tempFilename += filename.replace(".txt","");
            File outputFile = new File(tempFilename+"_path.png");
            ImageIO.write(image, "PNG", outputFile);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            graphics.dispose();
        }
    }
    /**
     * That writes path to txt file
     * @param path variable that holds path
     */
    public void writePath(List path){
        // Write the path to a file
        try {
            String folderName = "outputs";
            String tempFilename = "";
            // Get the current directory
            String currentDirectory = System.getProperty("user.dir");
            // Create a File object representing the new folder
            File newFolder = new File(currentDirectory + File.separator + folderName);
            if(newFolder.exists()){
                tempFilename += "outputs/";
            }
            // Create the new folder
            else if(newFolder.mkdir()) {
                tempFilename += "outputs/";

            }

            tempFilename += filename.replace(".txt","");

            File myFile = new File(tempFilename+"_path.txt");

            myFile.createNewFile();
            FileWriter myWriter = new FileWriter(tempFilename+"_path.txt");

            for(Object point : path) {
                int x = ((CSE222Graph.Point)point).getX();
                int y = ((CSE222Graph.Point)point).getY();
                myWriter.write(x + "," + y + "\n");
            }
            myWriter.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
 
    public int getX_SIZE() {
        return X_SIZE;
    }

    public int getY_SIZE() {
        return Y_SIZE;
    }

    public String getFilename() {
        return filename;
    }

    public ArrayList<ArrayList<Integer>> getMap() {
        return map;
    }

    public int getStart_x() {
        return start_x;
    }

    public void setStart_x(int start_x) {
        this.start_x = start_x;
    }

    public int getStart_y() {
        return start_y;
    }

    public void setStart_y(int start_y) {
        this.start_y = start_y;
    }

    public int getEnd_x() {
        return end_x;
    }

    public void setEnd_x(int end_x) {
        this.end_x = end_x;
    }

    public int getEnd_y() {
        return end_y;
    }

    public void setEnd_y(int end_y) {
        this.end_y = end_y;
    }
}
