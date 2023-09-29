/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 *
 */

public class DataPool {
    private final List<DataPoolType> Elements;
    public DataPool(){
        this.Elements = new List<>();
    }
    public void AddElement(DataPoolType Element){
        this.Elements.Append(Element);
    }
    public DataPoolType GetElement(int Index){
        return this.Elements.Get(Index);
    }
}
