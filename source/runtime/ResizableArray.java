/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class ResizableArray<T> {
    private Object[] Data;
    private int Size;

    public ResizableArray(){
        Size = 0;
        this.Data = new Object[10];
    }

    public T At(int Index){
        return (T)Data[Index];
    }
    public void Append(T Object){
        if (Data[Data.length-1] != null){
            Object[] Info = new Object[Data.length+5];
            for (int x = 0; x < Data.length; x++){
                Info[x] = Data[x];
            }
            this.Data = Info;
            Data[Size] = Object;
            Size++;
        }
        else {
            Data[Size] = Object;
            Size++;
        }
    }
    public int GetLength(){
        return Size;
    }
}
