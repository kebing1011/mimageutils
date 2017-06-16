package com.aspirecn.hawaii.image;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import javax.imageio.stream.FileImageOutputStream;

public final class ImageToolKit {
	static {
		System.loadLibrary("imagetoolkit");
	}

	/**
	 * resize image data (only PNG & JPG supported)
	 *
	 * @param imageBytes
	 *            input image data
	 * @param size
	 *            input resized size max height-width
	 * @param quality
	 *            quality (1-100) max 100 advice use 75 (JPG-only, PNG No
	 *            effect)
	 *
	 * @return output image data or null if error.
	 */

	public static native byte[] resize(byte[] imageBytes, int size, int quality);


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
		
		String jpg1file = "1.jpg";
		String jpg2file = "1-1.jpg";
		String png1file = "1.png";
		String png2file = "1-1.png";

		byte[] buf1 = null;
		byte[] buf2 = null;

		// test resize png
		buf1 = readFile(png1file);
		buf2 = resize(buf1, 300, 100);
		writeFile(png2file, buf2);
		System.out.println(png1file + " resize --> " + png2file + " sucess.");

		// test resize jpg
		buf1 = readFile(jpg1file);
		buf2 = resize(buf1, 300, 100);
		writeFile(jpg2file, buf2);
		System.out.println(jpg1file + " resize --> " + jpg2file + " sucess.");
	}

}
