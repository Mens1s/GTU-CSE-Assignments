import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
/**
 * CSE222BFS Class that finds best path as using BFS Algorithm
 */
public class CSE222BFS {
    private final int[][] directions = {{-1,0},{1,0},{0,-1},{0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}}; // {{-1,0},{1,0},{0,-1},{0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    /**
     * Graph variable that holds graph
    */
    private final CSE222Graph graph;
    /**
     * Name of algorithm
    */
    public final String name = "BFS";
    /**
     * Path length
    */
    public int length = 0;
    /**
     * constructof bfs class
     * @param graph variable of graph
     */
    public CSE222BFS(CSE222Graph graph) {
        this.graph = graph;
    }
    
    /**
     * Function trys to find best path as using BFS algorithm
     * @return List of CSE222.Point Class
     */
    public List<CSE222Graph.Point> findPath() {
        int startX = graph.getStartX();
        int startY = graph.getStartY();
        int endX = graph.getEndX();
        int endY = graph.getEndY();

        int x_size = graph.getX();
        int y_size = graph.getY();
        int[][] distances = new int[y_size][x_size];
        int[][] parent = new int[y_size][x_size];

        for (int i = 0; i < y_size; i++) {
            for (int j = 0; j < x_size; j++) {
                distances[i][j] = Integer.MAX_VALUE; // that will be represent not visited
            }
        }

        distances[startX][startY] = 0;

        Queue<CSE222Graph.Point> queue = new LinkedList<>();
        queue.add(new CSE222Graph.Point(startX, startY));

        while (!queue.isEmpty()) {
            CSE222Graph.Point current = queue.poll();
            int x = current.getX();
            int y = current.getY();

            if (x == endX && y == endY) {
                // Found the end point, reconstruct the path
                length = distances[x][y];
                return reconstructPath(parent, startX, startY, endX, endY);
            }

            for (int[] dir : directions) {
                int newX = x + dir[0];
                int newY = y + dir[1];

                if (isValidMove(newX, newY, x_size, y_size) && distances[newX][newY] == Integer.MAX_VALUE) {
                    queue.add(new CSE222Graph.Point(newX, newY));
                    distances[newX][newY] = distances[x][y] + 1;
                    parent[newX][newY] = x * x_size + y;
                }
            }
        }

        return new ArrayList<>(); // No feasible path found
    }
    /**
     * Function tries to position is valid or not
     * @param x x variable that will be go
     * @param y y variable that will be go
     * @param x_size x size of graph
     * @param y_size y size of graph
     * @return isValid position or not
     */
    private boolean isValidMove(int x, int y, int x_size, int y_size) {
        return x >= 0 && x < x_size && y >= 0 && y < y_size && graph.getMap().getMap().get(y).get(x) == 0;
    }
    /**
     * It builds path by using previous x, y values, it goes until position starx and starty
     * @param parent array of x values
     * @param start_x start position of x
     * @param start_y start position of y
     * @param end_x end position of x
     * @param end_y end position of y
     * @return List of CSE222.Point Class
     */
    private List<CSE222Graph.Point> reconstructPath(int[][] parent, int startX, int startY, int endX, int endY) {
        List<CSE222Graph.Point> path = new ArrayList<>();
        int x_size = graph.getX();
        int y_size = graph.getY();
        int x = endX;
        int y = endY;

        while (x != startX || y != startY) {
            path.add(0, new CSE222Graph.Point(x, y));

            int parentX = parent[x][y] / x_size;
            int parentY = parent[x][y] % y_size;
            x = parentX;
            y = parentY;
        }

        path.add(0, new CSE222Graph.Point(startX, startY));
        return path;
    }
}
