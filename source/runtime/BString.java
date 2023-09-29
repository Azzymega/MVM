/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class BString {
    private char[] Characters;

    public BString(char[] characters) {
        Characters = characters;
    }

    public char[] getCharacters() {
        return Characters;
    }

    public void setCharacters(char[] characters) {
        Characters = characters;
    }
}
