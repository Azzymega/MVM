/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 *
 */

public class DataPoolType {
    private final short TypeTag;
    private ClassLink ClassLink;
    private MethodLink MethodLink;
    private FieldLink FieldLink;
    private double Double;
    private BString String;
    private long Long;
    private float Float;
    private int Integer;
    private Boiler Boiler;

    public DataPoolType(short typeTag) {
        TypeTag = typeTag;
    }

    public MethodLink getMethodLink() {
        return MethodLink;
    }

    public void setMethodLink(MethodLink methodLink) {
        MethodLink = methodLink;
    }

    public ClassLink getClassLink() {
        return ClassLink;
    }

    public void setClassLink(ClassLink classLink) {
        ClassLink = classLink;
    }

    public FieldLink getFieldLink() {
        return FieldLink;
    }

    public void setFieldLink(FieldLink fieldLink) {
        FieldLink = fieldLink;
    }

    public double getDouble() {
        return Double;
    }

    public void setDouble(double aDouble) {
        Double = aDouble;
    }

    public BString getString() {
        return String;
    }

    public void setString(BString string) {
        String = string;
    }

    public long getLong() {
        return Long;
    }

    public void setLong(long aLong) {
        Long = aLong;
    }

    public float getFloat() {
        return Float;
    }

    public void setFloat(float aFloat) {
        Float = aFloat;
    }

    public int getInteger() {
        return Integer;
    }

    public void setInteger(int integer) {
        Integer = integer;
    }

    public Boiler getBoiler() {
        return Boiler;
    }

    public void setBoiler(Boiler boiler) {
        Boiler = boiler;
    }

    public short getTypeTag() {
        return TypeTag;
    }
}
