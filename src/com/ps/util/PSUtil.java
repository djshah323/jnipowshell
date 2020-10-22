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
	public static void execute(String user, String password, String domain, String commandlet) {
		// TODO Auto-generated method stub
		PSUtil newUtil = new PSUtil();
		newUtil.init(domain, user, password);
		String ret = newUtil.invoke(domain, commandlet);
		System.out.println(ret);
	}

}
