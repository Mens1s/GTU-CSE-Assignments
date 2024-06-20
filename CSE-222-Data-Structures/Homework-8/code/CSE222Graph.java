
/**
 * Graph Class that represents CSE222Map class as a graph
 */
public class CSE222Graph {
    /**
     * Map variable that holds of Map
    */
    private final CSE222Map map;
    /**
     * Directions variable that hold instant edge can go
    */
    private final int [][] directions = {{-1,0},{1,0},{0,-1},{0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    /**
     * X_SIZE of Map
    */
    private final int x;
    /**
     * Y_SIZE of Map
    */
    private final int y;
    /**
     * X point of start point
    */
    private final int startX;
    /**
     * Y point of start point
    */
    private final int startY;
    /**
     * X point of end point
    */
    private final int endX;
    /**
     * Y point of end point
    */
    private final int endY;
    /**
     * Constructor of Graph Class
     * @param Map variable that holds map
     */
    public CSE222Graph(CSE222Map Map){
        this.map = Map;
        this.x = map.getX_SIZE();
        this.y = map.getY_SIZE();
        this.startX = map.getStart_x();
        this.startY = map.getStart_y();
        this.endX = map.getEnd_x();
        this.endY = map.getEnd_y();
    }

    /**
     * Static class that holds information about Point
     */
    public static class Point implements Comparable<Point>{
        /**
         * X point 
        */
        private final int x;
        /**
         * Y point of
        */
        private final int y;

        public Point(int x, int y){
            this.x = x;
            this.y = y;
        }
        @Override
        public int compareTo(Point other) {
            // Compare points based on their priorities
            // Return a negative value if this point has higher priority,
            // a positive value if the other point has higher priority,
            // or 0 if they have equal priority.
            // Implement the comparison logic based on your requirements.

            // Example comparison based on the sum of x and y coordinates:
            int thisPriority = this.x + this.y;
            int otherPriority = other.x + other.y;

            return Integer.compare(thisPriority, otherPriority);
        }
        public int getX() {
            return x;
        }

        public int getY() {
            return y;
        }

        
    }

    public CSE222Map getMap() {
        return map;
    }

    public int[][] getDirections() {
        return directions;
    }

    public int getX() {
        return x;
    }
    public int getY(){return y;}

    public int getStartX() {
        return startX;
    }

    public int getStartY() {
        return startY;
    }

    public int getEndX() {
        return endX;
    }

    public int getEndY() {
        return endY;
    }
}
