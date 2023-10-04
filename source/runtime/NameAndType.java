/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class NameAndType {
    private DataPoolType Name;
    private DataPoolType Descriptor;

    public NameAndType(DataPoolType name, DataPoolType descriptor) {
        Name = name;
        Descriptor = descriptor;
    }

    public String getName() {
        return Name.getString();
    }

    public void setName(String name) {
        Name.setString(name);
    }

    public DataPoolType getDescriptor() {
        return Descriptor;
    }

    public void setDescriptor(String descriptor) {
        Descriptor.setString(descriptor);
    }
}
