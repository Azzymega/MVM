public class List<T>{
    private Node<T> StartNode;
    private int Length;
    public List(){
        Length = 0;
    }
    public int GetLength(){
        return this.Length;
    }
    public void Append(T Object){
        if (StartNode == null){
            StartNode = new Node<T>(Object);
            Length++;
            return;
        }
        else{
            Node<T> RightNode = StartNode;
            for (int i = 0; i < Length; i++){
                RightNode = RightNode.GetNextNode();
            }
            RightNode = new Node<T>(Object);
            Length++;
            return;
        }
    }
    public T Get(int Place){
        Node<T> RightNode = StartNode;
        for (int i = 0; i < Place; i++){
            RightNode = RightNode.GetNextNode();
        }
        return RightNode.GetObject();
    }
}
