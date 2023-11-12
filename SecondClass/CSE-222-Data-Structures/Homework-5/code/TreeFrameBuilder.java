import javax.swing.*;
import javax.swing.tree.DefaultMutableTreeNode;
import java.io.File;
import java.util.Scanner;
import java.util.Queue;
import java.util.LinkedList;
import java.util.Stack;
/**
 * TreeFrameBuilder is a class that automatically created tree from tree.txt
 * It includes search algorithms such as DFS, BFS and Traverse
 * It ıncludes also move method too.*/
public class TreeFrameBuilder {
    /**
     * 2D String array that holds all dirs in tree.txt*/
    private String [][] directories;
    /**
     * JFrame frame variable that holds dirs as a tree*/
    private JFrame frame;
    /**
     * Root of tree*/
    private DefaultMutableTreeNode root;
    /**
     * Constructor of TreeFrameBuilder class. It calls setSize and getDirs methods*/
    public TreeFrameBuilder(){
        setSize();
        getDirs();
    }
    /**
     * SetSize method sets sizes of directories variables. Dynamically reallocated.*/
    private void setSize(){
        try{
            File myDirs = new File("tree.txt");
            Scanner reader = new Scanner(myDirs);

            int rNumber = 0;
            while (reader.hasNextLine()){
                reader.nextLine();
                rNumber++;
            }
            reader.close();
            directories = new String[rNumber][];

            int n = 0;
            Scanner newReader = new Scanner(myDirs);

            while (newReader.hasNextLine()){

                String data = newReader.nextLine();
                directories[n] = new String[data.split(";").length];

                n++;
            }
            newReader.close();
        }catch (java.io.FileNotFoundException e){
            System.out.println(e.getMessage());
        }
    }
    /**
     * GetDirs methods read txt and put information in directories array. It calls toJTree method.*/
    private void getDirs(){
        try{
            File myDirs = new File("tree.txt");
            Scanner reader = new Scanner(myDirs);

            int number = 0;
            while (reader.hasNextLine()){

                String data = reader.nextLine();
                int n = 0;
                for( String info : data.split(";") ){

                    directories[number][n] = info;
                    n++;
                }

                number++;
            }

            reader.close();
            toJTree();
        }catch (java.io.FileNotFoundException e){
            System.out.println(e.getMessage());
        }
    }
    /**
     * toJTree method creates tree with directories variable information.*/
    private void toJTree(){
        root = new DefaultMutableTreeNode("Root");

        for(String [] dir : directories){
            DefaultMutableTreeNode pNode = root;

            for(int i = 0; i < dir.length - 1; i++){
                String nName = dir[i];
                boolean isUsed = false;

                for(int childIdx = 0; childIdx < pNode.getChildCount(); childIdx++){
                    DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) pNode.getChildAt(childIdx);
                    if (childNode.getUserObject().equals(nName)) {
                        pNode = childNode;
                        isUsed = true;
                        break;
                    }
                }

                if(!isUsed) {
                    DefaultMutableTreeNode newNode = new DefaultMutableTreeNode(nName);
                    pNode.add(newNode);
                    pNode = newNode;
                }
            }

            String childName = dir[dir.length-1];
            boolean isUsed = false;

            for(int childIdx = 0; childIdx < pNode.getChildCount(); childIdx++) {
                DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) pNode.getChildAt(childIdx);
                if (childNode.getUserObject().equals(childName)) {
                    isUsed = true;
                    break;
                }
            }

            if(!isUsed) {
                pNode.add(new DefaultMutableTreeNode(childName));
            }
        }

        frame = new JFrame();
        JTree jt = new JTree(root);
        frame.add(jt);
        frame.setSize(600, 800);
        frame.setVisible(true);
    }
    /**
     * @param target String value that we search in tree
     * It includes BFS search algorithm.*/
    public void BFS(String target) {
        if(root == null) return;

        System.out.println("Using BFS to find '"+target+"' in the tree...");
        Queue<DefaultMutableTreeNode> queue = new LinkedList<>();
        queue.offer(root);

        int step = 1;
        boolean isFound = false;

        while(!queue.isEmpty() && !isFound) {
            DefaultMutableTreeNode tempNode = queue.poll();

            String value = (String) tempNode.getUserObject();
            if(value.equals(target)) isFound = true;
            System.out.print("\nStep "+step+" -> "+value);

            for(int i = 0; i <  tempNode.getChildCount(); i++) queue.offer((DefaultMutableTreeNode) tempNode.getChildAt(i));

            step++;
        }

        if(isFound) System.out.println("(Found!)");
        else System.out.println("\nNOT FOUND!");
    }
    /**
     * @param target String value that we search in tree
     * It includes DFS search algorithm, we call DFS(node, target)*/
    public void DFS(String target) {
        System.out.println("Using DFS to find '"+target+"' in the tree...");

        if(DFS(root, target)) System.out.println("(Found!)");
        else System.out.println("\nNot Found!");
    }
    /**
     * @param target String value that we search in tree
     * @param node DefaultMutableTreeNode that modified node
     * It includes DFS search algorithm
     * @return it found or not*/
    private boolean DFS(DefaultMutableTreeNode node, String target) {
        Stack<DefaultMutableTreeNode> stack = new Stack<>();
        stack.push(node);
        int step = 1;

        while(!stack.isEmpty()) {
            DefaultMutableTreeNode cNode = stack.pop();
            String value = (String) cNode.getUserObject();
            System.out.print("\nStep "+step+" -> "+ value);

            if(value.equals(target)) return true;

            for(int i = cNode.getChildCount() -1;i >= 0; i--) stack.push( (DefaultMutableTreeNode) cNode.getChildAt(i));

            step++;
        }
        return false;
    }
    // recursion DFS step must be global var
    /**
     * @param target String value that we search in tree
     * @param node DefaultMutableTreeNode that modified node for recursive DFS search
     * It includes DFS search algorithm
     * @return it found or not*/
    private boolean DFS(DefaultMutableTreeNode node, String target, int step) {
        String value = (String) node.getUserObject();
        System.out.print("\nStep "+step+" -> "+ value);

        if(value.equals(target)) return true;

        else{
            for(int i = 0; i < node.getChildCount(); i++){
                if(DFS( (DefaultMutableTreeNode) node.getChildAt(i), target, ++step))return true;
            }
        }
        return false;
    }
    /**
     * @param target String value that we search in tree
     * It includes Traverse search algorithm*/
    public void Traverse(String target) {
        if(root == null) return;

        Stack<DefaultMutableTreeNode> nodes = new Stack<>();
        Stack<DefaultMutableTreeNode> reversedStack = new Stack<>();
        nodes.push(root);

        while(!nodes.isEmpty()) {
            DefaultMutableTreeNode c = nodes.pop();
            reversedStack.push(c);

            for(int i= 0; i < c.getChildCount();i++) nodes.push( (DefaultMutableTreeNode) c.getChildAt(i));
        }

        int step = 1;
        boolean isFound = false;
        while(!reversedStack.isEmpty()) {
            String value = (String) reversedStack.pop().getUserObject();
            System.out.print("\nStep "+step+" -> "+ value);
            if(value.equals(target)) {
                isFound = true;
                break;
            }
            step++;
        }
        if(isFound) System.out.println("(Found!)");
        else System.out.println("\nNot Found!");
    }
    /**
     * @param from String value that we copy path of the node
     * @param to String value that we move path of the copied node
     * It includes Copy method algorithm*/
    public void Move(String from, String to) {
        boolean isExist = false;
        String [] fPath = from.split("->");
        int fPathCount = fPath.length;

        String [] tPath = to.split("->");
        int tPathCount = tPath.length;

        if(fPathCount == 1) {
            boolean toPathTrue = false;
            int idx = 0;
            int idx2 = 0;

            for(int i = 0; i < root.getChildCount();i++) {
                if(equals(root.getChildAt(i).toString(), fPath[0] )){
                    isExist = true;
                    idx = i;
                }
                if(equals(root.getChildAt(i).toString(), tPath[0])) {
                    toPathTrue = true;
                    idx2= i;
                }
            }
            if(isExist) {
                if(toPathTrue) {
                    DefaultMutableTreeNode movedNode = (DefaultMutableTreeNode) root.getChildAt(idx);
                    root.remove(idx);

                    DefaultMutableTreeNode parentNode = (DefaultMutableTreeNode) root.getChildAt(idx2);
                    parentNode.add(movedNode);
                }
                else{
                    DefaultMutableTreeNode movedNode = (DefaultMutableTreeNode) root.getChildAt(idx);
                    root.remove(idx);

                    DefaultMutableTreeNode parentNode = new DefaultMutableTreeNode(tPath[0]);
                    root.add(parentNode);
                    parentNode.add(movedNode);
                }
                System.out.println("File is transfered!");
                frame = new JFrame();
                JTree jt = new JTree(root);
                frame.add(jt);
                frame.setSize(600, 800);
                frame.setVisible(true);
            }else
                System.out.println("Cannot move " + from + " because it doesn't exist in the tree.");



            return;
        }
        // updating target path
        if(tPathCount - 1 < fPathCount ) {
            // new Path
            String [] targetPath = new String[fPathCount - 1];

            for(int i = 0; i < tPathCount; i++)
                targetPath[i] = tPath[i];

            for(int i = tPathCount ; i < fPathCount - 1; i++)
                targetPath[i] = fPath[i];

            tPath = targetPath;
            tPathCount = tPath.length;
        }
        DefaultMutableTreeNode cNode = root;
        DefaultMutableTreeNode movedNode = null;
        for(int i = 0; i < fPathCount; i++){
            boolean isPathCorrect = false;
            for(int j = 0; j < cNode.getChildCount(); j++){

                if(equals(cNode.getChildAt(j).toString(), fPath[i])) {
                    if(i == fPathCount-1) {
                        isExist = true;
                        // remove and move ops
                        movedNode = (DefaultMutableTreeNode) cNode.getChildAt(j);
                        cNode.remove(j);
                        if(cNode.getChildCount() == 0) cNode.removeFromParent();
                    }else{
                        isPathCorrect = true;
                        cNode = (DefaultMutableTreeNode) cNode.getChildAt(j);
                    }
                    break;
                }
            }
            if(!isPathCorrect) break;
        }
        // move
        if(isExist){
            boolean isOverWritten = false;
            cNode = root;

            for(int i = 0; i < tPathCount; i++) {
                if(cNode.getChildCount() == 0) {
                    System.out.println("New Path as named -> " + tPath[i] + " created.");
                    DefaultMutableTreeNode parentNode = new DefaultMutableTreeNode(tPath[i]);
                    cNode.add(parentNode);
                    cNode = parentNode;
                    if (i == tPathCount - 1) {
                        parentNode.add(movedNode);
                    }
                }
                else {
                    for (int j = 0; j < cNode.getChildCount(); j++) {
                        if (equals(cNode.getChildAt(j).toString(), tPath[i])) {
                            if (i == tPathCount - 1) {
                                DefaultMutableTreeNode parentNode = (DefaultMutableTreeNode) cNode.getChildAt(j);
                                // CONTROLLING OF overwritting
                                int d = 0;
                                for(;d < parentNode.getChildCount(); d++){
                                    if(equals(parentNode.getChildAt(d).toString(), fPath[fPathCount - 1])){
                                        isOverWritten = true;
                                        break;
                                    }
                                }
                                parentNode.remove(d);
                                parentNode.add(movedNode);
                            } else {
                                cNode = (DefaultMutableTreeNode) cNode.getChildAt(j);
                            }

                            break;
                        }
                        // create New Node
                        else if (j == cNode.getChildCount() - 1) {
                            System.out.println("New Path as named -> " + tPath[i] + " created.");
                            DefaultMutableTreeNode parentNode = new DefaultMutableTreeNode(tPath[i]);
                            cNode.add(parentNode);
                            cNode = parentNode;
                            if (i == tPathCount - 1) {
                                parentNode.add(movedNode);
                            }
                        }
                    }
                }
            }
            System.out.println("Moved " + from + " to " + to);
            if(isOverWritten) {
                for(String s : tPath) System.out.print(s+"->");
                System.out.println(" has been overwritten");
            }
            //frame.setVisible(false);
            frame = new JFrame();
            JTree jt = new JTree(root);
            frame.add(jt);
            frame.setSize(600, 800);
            frame.setVisible(true);
        }
        else System.out.println("Cannot move " + from + " because it doesn't exist in the tree.");
    }
    /**
     * @param a Object value that I compare
     * @param b Object value that I compare
     * @return equality of object a and b*/
    private boolean equals(Object a, Object b) {
        if( a == b) return true;
        if (a == null ||b == null) return false;
        return a.equals(b);
    }

}
