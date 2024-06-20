import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Arrays;

/**
 * CSE222Dijkstra Class that finds best path as using Dijikstra Algorithm
 */
public class CSE222Dijkstra {
    /**
     * Graph variable that holds graph
    */
    private final CSE222Graph graph;
    /**
     * Boolean 2D array that holds is point is visited or not
    */
    private final boolean[][] visited;
    /**
     * Start point of Map as X
    */
    private final int startX;
    /**
     * Start point of Map as Y
    */
    private final int startY;
    /**
     * End point of Map as X
    */
    private final int endX;
    /**
     * End point of Map as Y
    */
    private final int endY;
    /**
     * Path length
    */
    private int length;
    /**
     * Name of algorithm
    */
    public final String name = "Dijkstra";
    /**
     * Constructor of Djikstra Class
     * @param graph variable that holds graph
     */
    public CSE222Dijkstra(CSE222Graph graph) {
        this.graph = graph;
        this.visited = new boolean[graph.getY()][graph.getX()];
        this.startX = graph.getStartX();
        this.startY = graph.getStartY();
        this.endX = graph.getEndX();
        this.endY = graph.getEndY();
        this.length = 0;
    }
    /**
     * Function trys to find best path as using Dijikstra algorithm
     * @return List of CSE222.Point Class
     */
    public List<CSE222Graph.Point> findPath() {
        // Initialize visited array
        for (int i = 0; i < graph.getY(); i++) {
            Arrays.fill(visited[i], false);
        }

        int[][] distance = new int[graph.getY()][graph.getX()];
        int[][] prevX = new int[graph.getY()][graph.getX()];
        int[][] prevY = new int[graph.getY()][graph.getX()];

        for (int i = 0; i < graph.getY(); i++) {
            Arrays.fill(distance[i], Integer.MAX_VALUE);
        }

        PriorityQueue<CSE222Graph.Point> queue = new PriorityQueue<>(Comparator.comparingInt(p -> distance[p.getY()][p.getX()]));

        distance[startY][startX] = 0;
        queue.offer(new CSE222Graph.Point(startX, startY));

        while (!queue.isEmpty()) {
            CSE222Graph.Point curr = queue.poll();
            int currX = curr.getX();
            int currY = curr.getY();

            if (currX == endX && currY == endY) {
                break; // Reached the end point
            }

            if (visited[currY][currX]) {
                continue; // Already visited
            }

            visited[currY][currX] = true;

            for (int[] dir : graph.getDirections()) {
                int neighborX = currX + dir[0];
                int neighborY = currY + dir[1];

                if (isValidPosition(neighborX, neighborY) && !visited[neighborY][neighborX]) {
                    int newDistance = distance[currY][currX] +1;

                    if (newDistance < distance[neighborY][neighborX]) {
                        distance[neighborY][neighborX] = newDistance;
                        prevX[neighborY][neighborX] = currX;
                        prevY[neighborY][neighborX] = currY;
                        queue.offer(new CSE222Graph.Point(neighborX, neighborY));
                    }
                }
            }
        }

        return buildPath(prevX, prevY);
    }
    /*public List<CSE222Graph.Point> findPath() {
        // Initialize visited array
        for (int i = 0; i < graph.getY(); i++) {
            for (int j = 0; j < graph.getX(); j++) {
                visited[i][j] = false;
            }
        }
        int[][] distance = new int[graph.getY()][graph.getX()];

        // Dijkstra's algorithm using BFS 
        //Queue<CSE222Graph.Point> queue = new LinkedList<>();
        PriorityQueue<CSE222Graph.Point> queue = new PriorityQueue<>(Comparator.comparingInt(p -> distance[p.getY()][p.getX()]));
        distance[startY][startX] = 0;

        queue.offer(new CSE222Graph.Point(startX, startY));
        visited[startY][startX] = true; // holds as visited

        int[][] prevX = new int[graph.getY()][graph.getX()]; 
        int[][] prevY = new int[graph.getY()][graph.getX()];

        while (!queue.isEmpty()) { // until no way to go
            CSE222Graph.Point curr = queue.poll();
            int currX = curr.getX();
            int currY = curr.getY();

            if (currX == endX && currY == endY) {
                break;  // Reached the end point
            }


            for (int[] dirr : graph.getDirections()) {
                int neighborX = currX + dirr[1];
                int neighborY = currY + dirr[0];

                if (isValidPosition(neighborX, neighborY) && !visited[neighborY][neighborX]) {
                    int newDistance = distance[currY][currX] + 1;

                    if(newDistance < distance[neighborY][neighborX]){
                        distance[neighborY][neighborX] = newDistance;
                        queue.offer(new CSE222Graph.Point(neighborX, neighborY));
                        visited[neighborY][neighborX] = true;
                        prevX[neighborY][neighborX] = currX;
                        prevY[neighborY][neighborX] = currY;
                    }
                    
                }
            }
        }

        return buildPath(prevX, prevY);
    }*/
    /**
     * Function tries to position is valid or not
     * @param x x variable that will be go
     * @param y y variable that will be go
     * @return isValid position or not
     */
    private boolean isValidPosition(int x, int y) {
        return x >= 0 && x < graph.getX() && y >= 0 && y < graph.getY() && graph.getMap().getMap().get(y).get(x) == 0;
    }
    /**
     * It builds path by using previous x, y values, it goes until position starx and starty
     * @param prevX array of x values
     * @param prevY array of y values
     * @return List of CSE222.Point Class
     */
    private List<CSE222Graph.Point> buildPath(int[][] prevX, int[][] prevY) {
        List<CSE222Graph.Point> path = new ArrayList<>();
        int currX = endX;
        int currY = endY;

        while (currX != startX || currY != startY) {
            path.add(0, new CSE222Graph.Point(currX, currY));
            int parentX = prevX[currY][currX];
            int parentY = prevY[currY][currX];
            currX = parentX;
            currY = parentY;
        }

        path.add(0, new CSE222Graph.Point(startX, startY));
        length = path.size() - 1;

        return path;
    }

    public int getLength() {
        return length;
    }
}
