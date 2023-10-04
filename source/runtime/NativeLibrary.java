/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

// (Wrapper over HMODULE)
public class NativeLibrary {
    private String libraryName;

    public native byte[] LoadNativeLibrary(String libraryName);
    public NativeLibrary(String libraryName) {
        this.libraryName = libraryName;
    }
    public String getLibraryName() {
        return libraryName;
    }

    public void setLibraryName(String libraryName) {
        this.libraryName = libraryName;
    }
}
