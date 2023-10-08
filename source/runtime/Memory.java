/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class Memory {
    public native long alloca(int size);
    public native void free(long ptr);
    public native void memcpy(byte[] destination, byte[] source, int length);
    public native boolean memcmp(byte[] destination, byte[] source, int length);
}
