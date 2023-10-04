/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class ClassHeader {
    private short MinimalVersion;
    private short MaximumVersion;
    private List<AccessFlag> Flags;
    private Class ThisClass;
    private Class SuperClass;

    public ClassHeader(short minimalVersion, short maximumVersion, List<AccessFlag> flags, Class thisClass, Class superClass) {
        MinimalVersion = minimalVersion;
        MaximumVersion = maximumVersion;
        Flags = flags;
        ThisClass = thisClass;
        SuperClass = superClass;
    }

    public Class getSuperClass() {
        return SuperClass;
    }

    public void setSuperClass(Class superClass) {
        SuperClass = superClass;
    }

    public Class getThisClass() {
        return ThisClass;
    }

    public void setThisClass(Class thisClass) {
        ThisClass = thisClass;
    }

    public List<AccessFlag> getFlags() {
        return Flags;
    }

    public void setFlags(List<AccessFlag> flags) {
        Flags = flags;
    }

    public short getMaximumVersion() {
        return MaximumVersion;
    }

    public void setMaximumVersion(short maximumVersion) {
        MaximumVersion = maximumVersion;
    }

    public short getMinimalVersion() {
        return MinimalVersion;
    }

    public void setMinimalVersion(short minimalVersion) {
        MinimalVersion = minimalVersion;
    }
}
