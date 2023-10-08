/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class System {

    public native static void system(String command);
    public native static void printf(String message);
    public static void main(String[] args) {
        String str = "System.dll";
        NativeLibrary library = new NativeLibrary(str);
        library.LoadNativeLibrary(library.getLibraryName());
        String ggg = "hello";
        printf(ggg); // testing
    }
}
