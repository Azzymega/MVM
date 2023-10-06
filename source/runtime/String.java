/*
 *
 *  * PROJECT:     MVM - JAVA STDLIB
 *  * LICENSE:     GPL - See COPYING in the top level directory
 *  * PROGRAMMER:  Maltsev Daniil <brickexberiment@lenta.ru>
 * /
 */


package java.lang;



public class String {
	private byte value[]; // Only ASCII now
	public String(){
		value = new byte[0];
	}
	public byte[] returnValue(){
		return value;
	}
	public String(String value) {
		this.value = value.returnValue();
    }
	public String(byte value[]){
		this.value = value;
	}
}