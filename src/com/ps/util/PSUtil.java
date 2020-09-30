/**
 * 
 */
package com.ps.util;

/**
 * @author SDhaval
 *
 */
public class PSUtil {

	static {
		System.loadLibrary("libps");
	}
	
	public native void init(String tenant, String username, String password);
	
	public native String invoke(String tenant, String command);
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		PSUtil newUtil = new PSUtil();
		newUtil.init("azure.onmicrosoft.com", "user", "password");
		String ret = newUtil.invoke("azure.onmicrosoft.com", "Get-AcceptedDomain");
		System.out.println(ret);
	}

}
