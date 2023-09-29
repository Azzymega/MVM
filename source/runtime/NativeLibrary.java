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
    private byte[] libraryInternals;

    public NativeLibrary(String libraryName, int LibrarySize) {
        this.libraryName = libraryName;
        this.libraryInternals = new byte[LibrarySize];
    }

    public byte[] getLibraryInternals() {
        return libraryInternals;
    }

    public void setLibraryInternals(byte[] libraryInternals) {
        this.libraryInternals = libraryInternals;
    }

    public String getLibraryName() {
        return libraryName;
    }

    public void setLibraryName(String libraryName) {
        this.libraryName = libraryName;
    }
}
