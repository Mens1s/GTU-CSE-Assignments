import java.awt.*;
import java.util.List;

public class TestCases implements Runnable {
    /**
     * FileName */ 
    private String FileName;
    /**
     * X_SIZE of Map
     */
    private int X_SIZE;
    /**
     * Y_SIZE of Map
     */
    private int Y_SIZE;
    /**
     * Test Case constructor
     * @param FileName Filename of file
     * @param X_SIZE x size of map
     * @param Y_SIZE y size of map
     */
    public TestCases(String FileName, int X_SIZE, int Y_SIZE) {
        //if(X_SIZE < 0 || Y_SIZE < 0) {
        //    System.out.println("Sizes cannot be under 0. Program terminated");
        //    return;
        //}
        this.FileName = FileName;
        this.X_SIZE = X_SIZE;
    	this.Y_SIZE = Y_SIZE;
    }



  
    /**
     * Test functoion of TestCases
     */
    public void test(){
    	
    	System.out.println("\n\n*******************\nMap is " + this.FileName + " with X_SIZE " + this.X_SIZE + " and Y_SIZE " + this.Y_SIZE + "\n********************\n");
        
        //running test cases
        CSE222Map Map = new CSE222Map (this.FileName, this.X_SIZE, this.Y_SIZE);
        CSE222Graph Graph = new CSE222Graph (Map);

        CSE222Dijkstra Dijkstra = new CSE222Dijkstra (Graph);
        CSE222BFS BFS= new CSE222BFS (Graph);

        List BFSPath = BFS.findPath();
        List DijkstraPath = Dijkstra.findPath();

        Map.convertPNG();
        
        Map.drawLine(BFSPath, Color.RED);
        Map.drawLine(DijkstraPath, Color.GREEN);

        Map.writePath(BFSPath);
        Map.writePath(DijkstraPath);

        if(BFS.length == 0 || Dijkstra.getLength() == 0) 
            System.out.println("No path found!");

        else{
            System.out.println("****** Map : " + FileName + "********");
            System.out.println("BFS Path: "+ BFS.length);
            System.out.println("Dijkstra Path: "+ Dijkstra.getLength());
        }

    }

    @Override
    public void run() {
        test();
    }
}

