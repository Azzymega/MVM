/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class ClassLink {
    private Class ClassLink;
    private DataPoolType ClassName;

    public ClassLink(Class classLink, DataPoolType className) {
        ClassLink = classLink;
        ClassName = className;
    }

    public String getClassName() {
        return ClassName.getString();
    }

    public void setClassName(String className) {
        ClassName.setString(className);
    }

    public Class getClassLink() {
        return ClassLink;
    }

    public void setClassLink(Class classLink) {
        ClassLink = classLink;
    }
}
