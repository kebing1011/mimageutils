/**
 *
 * 图片工具类
 * 版本 1.0
 * 作者 Maokebing
 * 时间 2017.08.24
 * 描述 支持 JPG & PNG & WebP 格式图片缩放,转换
 * 
 */

package com.aspirecn.mao;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public final class ImageTools {
	static {
		System.loadLibrary("imagetools");
	}

	/**
	 * resize image data (only PNG,JPG,WebP supported)
	 *
	 * @param bytes
	 *            input image data
	 * @param size
	 *            input resized size max height-width
	 * @param quality
	 *            quality (1-100) max 100 advice use 75 (JPG-only, PNG No
	 *            effect)
	 *
	 * @return output image data or null if error.
	 */

	public static native byte[] resize(byte[] bytes, int size, int quality);


	/**
	 * convert image data from format WebP to JPG
	 *
	 * @param bytes
	 *            input image data from WebP format
	 * @param quality
	 *            quality (1-100) max 100 advice use 75 (JPG-only, PNG No
	 *            effect)
	 *
	 * @return output image data or null if error.
	 */

	public static native byte[] webp2jpg(byte[] bytes, int quality);


	// only for test code
	private static void writeFile(String filename, byte[] bytes) {
		if (bytes == null) {
			System.out.println("error!! bytes is null.");
			return;
		}
		try {
			FileOutputStream fos = new FileOutputStream(filename);
			fos.write(bytes);
			fos.close();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private static byte[] readFile(String filename) {
		File file = new File(filename);
		if (!file.exists()) {
			System.out.println("error!! " + filename + "is not exsits.");
			return null;
		}

		int size = (int) file.length();
		byte[] buffer = new byte[size];

		try {
			FileInputStream fis = new FileInputStream(filename);
			fis.read(buffer);
			fis.close();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return buffer;
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		System.out.println("test resize jpg");
		writeFile("1-resized.jpg", resize(readFile("1.jpg"), 200, 75));
		System.out.println("reszied jpg sucessed if nor error");

		System.out.println("test resize png");
		writeFile("1-resized.png", resize(readFile("1.png"), 200, 75));
		System.out.println("reszied png sucessed if nor error");

		System.out.println("test resize webp");
		writeFile("1-resized.webp", resize(readFile("1.webp"), 200, 75));
		System.out.println("reszied webp sucessed if nor error");

		System.out.println("test webp to jpg");
		writeFile("w2j.jpg", webp2jpg(readFile("1.webp"), 75));
		System.out.println("reszied webp sucessed if nor error");
	}

}
