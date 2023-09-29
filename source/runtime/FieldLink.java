/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class FieldLink {
    private TypeMetadata Data;
    private Field Field;

    public Field getField() {
        return Field;
    }

    public void setField(Field field) {
        Field = field;
    }

    public TypeMetadata getData() {
        return Data;
    }

    public void setData(TypeMetadata data) {
        Data = data;
    }
}
