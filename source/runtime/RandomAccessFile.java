/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

// (Wrapper over FILE)
public class RandomAccessFile {
    private final FileHandler handler;
    public RandomAccessFile(FileHandler handler) {
        this.handler = handler;
    }
    public native void readBytes(byte[] bytes, int len, FileHandler handler);
    public native void OpenStream(String params, FileHandler handler);
    public native void CloseStream(FileHandler handler);

    public FileHandler getHandler() {
        return handler;
    }
}
