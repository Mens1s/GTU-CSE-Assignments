package withLDLinkedList;
import java.util.*;
/**
 * The LDLinkedList Class implements linked list object.
 * LDLinkedList is type of linked list which inherited of List Abs
 *
 * @author Ahmet / mens1s
 * @version 0.1
 * @since 2023-04-04
 *
 */
public class LDLinkedList<E> extends AbstractList<E> implements List<E> {
    /**
     * head of list first element*/
    private Node<E> head;
    /**
     * tail of list last element */
    private Node<E> tail;
    /**
     * size of LDLinkedList */
    private int size;
    /**
     * count of deletedNodes for lazy deletion */
    private int deletedNodes = 0;
    /**
     * Static class of Node, node is an object of linked list objects*/
    private static class Node<E> {
        private E data;
        private Node<E> next;
        private boolean isDeleted;

        public Node(E data, Node<E> next){
            this.data = data;
            this.next = next;
            this.isDeleted = false;
        }
    }

    /**
     * Constructor of LDLinkedList class
     * Initially size = 0 and contains no element*/
    public LDLinkedList() {
        head = null;
        tail = null;
        size = 0;
    }
    /**
     * add(E Data) function adds data to tail
    * @param data Generic type data*/
    @Override
    public boolean add(E data) {
        Node<E> newNode = new Node<>(data, null);

        if(isEmpty()) head = tail = newNode;
        else tail = tail.next = newNode;

        size++;
        return true;
    }
    /**
     * Data(idx, data) function adds data to given index
     * @param data Generic type data
     * @param idx index of data located*/
    @Override
    public void add(int idx, E data) {
        controlIndexforAdding(idx);

        if(idx == size) add(data);
        else {
            Node<E> newNode = new Node<>(data, null);
            if(idx == 0) {
                newNode.next = head;
                head = newNode;
            } else {
                Node<E> prevNode = getNode(idx - 1);
                newNode.next = prevNode.next;
                prevNode.next = newNode;
            }
            size++;
        }
    }
    /**
     * given size of LDLinked list, it does not count lazy deleted nodes
     * @return size of list
    */
    @Override
    public int size() {
        return size;
    }
    /**
     * give the list is empty or not
     * @return list is empty or not
     */
    @Override
    public boolean isEmpty() {
        return size == 0;
    }
    /**
     * control of list contains object
     * @param o object that searched in list
     * @return list contains o Object
     */
    @Override
    public boolean contains(Object o) {
        return indexOf(o) != -1;
    }
    /**
     * control of idx of object
     * @param o object that searched in list
     * @return index of object located if it does not locate returns -1
     */
    @Override
    public int indexOf(Object o) {
        int idx = 0;

        for(Node<E> curr = head; curr != null; curr = curr.next, idx++)
            if(!curr.isDeleted && Objects.equals(curr.data, o) )
                return idx;

        return -1;
    }
    /**
     * given the data of given index
     * @param index index of object located
     * @return data of lacated object
     */
    @Override
    public E get(int index) {
        Node<E> curr = getNode(index);
        return curr.data;
    }
    /**
     * this function sets new data to given index and return old data
     * @param idx object located index
     * @param data new data
     * @return old data
     */
    @Override
    public E set(int idx, E data) {
        controlIndex(idx);
        Node<E> curr = head;
        curr  = getNode(idx);

        E temp = curr.data;
        curr.data = data;

        return temp;
    }
    /**
     * removes head of list
     * @return removed data
     */
    public E remove() {
        return remove(0);
    }
    /**
     * removes given index in list
     * @param index index of removed data
     * @return removed data
     */
    @Override
    public E remove(int index) {

        if(!controlIndex(index)) {return null;};
        Node<E> curr = getNode(index);

        curr.isDeleted = true;

        deletedNodes++;
        size--;

        if (size == 0) {
            head = null;
            tail = null;
        }

        if (deletedNodes % 2 == 0) {
            size += deletedNodes;
            removeDeletedNodes();
            size -= deletedNodes;
            deletedNodes = 0;
        }
        return curr.data;
    }
    /**
     * removes given object in list
     * @param o object to be removed
     * @return is removed or not
     */
    @Override
    public boolean remove(Object o) {
        int idx = indexOf(o);
        if(idx != -1){
            remove(idx);
            return true;
        }
        return false;
    }
    /**
     * lazy deletion step when it call it deletes lazy deleted nodes
     */
    private void removeDeletedNodes() {
        int idx = 0;

        for(Node<E> curr = head; curr != null; idx++, curr = curr.next){
           if(curr.isDeleted) {
               if(idx == 0) head = head.next;
               else if(curr == tail) tail = getNode(size - 1);
               else {
                   Node<E> prev = getNode(idx - 1);
                   prev.next = curr.next;
               }
               idx--;
           }
        }

    }
    /**
     * return given index node
     * @param index index of node object to be returned
     * @return Node object
     */
    private Node<E> getNode(int index) {
        controlIndex(index);

        Node<E> result = head;
        for (int idx = 0;idx < index;idx++, result = result.next){
            if(result.isDeleted) idx--;
        }
        if(result.isDeleted) result = result.next;
        return result;
    }
    /**
     * checks given index is suitable or not if not it throws an error
     * @param index checked integer
     */
    private boolean controlIndex(int index) {
        if(index < 0 || index >= size) return false;;
        return true;
    }
    /**
     * checks given index is suitable to add or not if not it throws an error
     * @param index checked integer
     */
    private void controlIndexforAdding(int index) {
        if(index < 0 || index > size) throw new IndexOutOfBoundsException("Out Of  Bounds Exception");
    }
    /**
     * special iterator for lazy deletion, it does not contain lazy deleted objects
     */
    public Iterator<E> iterator() {
        if (isEmpty()) {
            return Collections.<E>emptyList().iterator();
        }
        return new Iterator<E>() {
            private Node<E> curr = head;
            private Node<E> prev = null;

            @Override
            public boolean hasNext() {
                while (curr != null && curr.isDeleted) {
                    curr = curr.next;
                }
                return curr != null;
            }

            @Override
            public E next() {
                if (!hasNext()) {
                    throw new NoSuchElementException();
                }
                E element = curr.data;
                prev = curr;
                curr = curr.next;
                return element;
            }
        };
    }
    /**
     * special to String method for lazy deletion, it contains lazy deleted objects
     * */
    @Override
    public String toString() {
        String allNodes = "";
        Node<E> tempNode = head;

        while(tempNode != null) {
            allNodes += tempNode.data + " ";
            tempNode = tempNode.next;
        }

        return allNodes;
    }
}
