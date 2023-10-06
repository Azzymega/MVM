/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * 
 */

public class FileHandler {
    private byte[] FILE;
    private final String fileName;
    public FileHandler(String name){
        this.fileName = name;
    }

    public String getFileName() {
        return fileName;
    }
}
