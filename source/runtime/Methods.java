/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class Methods {
    private final List<MethodLink> Elements;
    public Methods(){
        this.Elements = new List<>();
    }
    public void AddElement(MethodLink Element){
        this.Elements.Append(Element);
    }
    public MethodLink GetElement(int Index){
        return this.Elements.Get(Index);
    }
}
