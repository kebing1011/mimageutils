package com.aspirecn.mao.image;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import javax.imageio.stream.FileImageOutputStream;

public final class ImageTools {
	static {
		System.loadLibrary("imagetools");
	}

	/**
	 * webp convert to jpeg
	 *
	 * @param webp
	 *            input webp data
	 * @param quality
	 *            quality (1-100) max 100 advice use 75
	 *
	 * @return output image data or null if error.
	 */

	public static native byte[] webp2jpg(byte[] webp, int quality);


	// use it for test ...
	public static void writeFile(String filename, byte[] bytes) {
		if (bytes == null) {
			System.out.println("bytes is null.");
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

	public static byte[] readFile(String filename) {
		File file = new File(filename);
		if (file == null) {
			System.out.println(filename + "is not exsits.");
			return null;
		}

		// small file
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
		
		String webpfile = "1.webp";
		String jpgfile = "1.jpg";

		// test webp2jpg png
		byte[] webpData = readFile(webpfile);
		byte[] jpgData = webp2jpg(webpData, 75);
		writeFile(jpgfile, jpgData);
		System.out.println(webpfile + " to jpg --> " + jpgfile + " sucess.");
	}

}
