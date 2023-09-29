/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */

public class MethodLink {
    private TypeMetadata Data;
    private Method Method;

    public MethodLink(TypeMetadata data, Method method) {
        Data = data;
        Method = method;
    }

    public Method getMethod() {
        return Method;
    }

    public void setMethod(Method method) {
        Method = method;
    }

    public TypeMetadata getData() {
        return Data;
    }

    public void setData(TypeMetadata data) {
        Data = data;
    }
}
