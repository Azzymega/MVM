/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class System {

    public native static void printf(char[] data);
    public static void main(String[] args) {
        String str = new String(new char[] {'S','y','s','t','e','m','.','d','l','l'});
        NativeLibrary library = new NativeLibrary(str);
        library.LoadNativeLibrary(library.getLibraryName());
        String ggg = new String(new char[] {'h','e','l','l','o'});
        printf(ggg.getCharacters()); // testing
    }
}
