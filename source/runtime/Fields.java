/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class Fields {
    private final List<FieldLink> Elements;
    public Fields(){
        this.Elements = new List<>();
    }
    public void AddElement(FieldLink Element){
        this.Elements.Append(Element);
    }
    public FieldLink GetElement(int Index){
        return this.Elements.Get(Index);
    }
}
