/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class String {
    private char[] Characters;

    public String(char[] characters) {
        Characters = characters;
    }

    public char[] getCharacters() {
        return Characters;
    }

    public void setCharacters(char[] characters) {
        Characters = characters;
    }
}
