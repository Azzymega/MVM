/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class AccessFlag {
    private final short Flag;
    public AccessFlag(short Flag){
        this.Flag = Flag;
    }
    public short GetFlag(){
        return Flag;
    }
}
