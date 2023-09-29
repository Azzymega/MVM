/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class Interfaces {
    private final List<ClassLink> Elements;
    public Interfaces(){
        this.Elements = new List<>();
    }
    public void AddElement(ClassLink Element){
        this.Elements.Append(Element);
    }
    public ClassLink GetElement(int Index){
        return this.Elements.Get(Index);
    }
}
