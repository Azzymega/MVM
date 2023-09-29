/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class TypeMetadata {
    private ClassLink Class;
    private NameAndType Information;

    public TypeMetadata(ClassLink aClass, NameAndType information) {
        Class = aClass;
        Information = information;
    }

    public NameAndType getInformation() {
        return Information;
    }

    public void setInformation(NameAndType information) {
        Information = information;
    }

    public ClassLink getThisClass() {
        return Class;
    }

    public void setClass(ClassLink aClass) {
        Class = aClass;
    }
}
