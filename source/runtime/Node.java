public class Node<T> {
    private T Object;
    private Node<T> NextNode;
    public Node(T Object){
        this.Object = Object;
    }
    public void SetNextNode(Node<T> NextNode){
        this.NextNode = NextNode;
    }
    public T GetObject(){
        return this.Object;
    }
    public void SetObject(T Object){
        this.Object = Object;
    }
    public Node<T> GetNextNode(){
        return this.NextNode;
    }
}
