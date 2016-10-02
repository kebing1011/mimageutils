//
//  resizepng.c
//  mjpeg
//
//  Created by Mao on 28/09/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include "resizepng.h"
#include "resizergb.h"
#include "rwpng.h"
#include <stdlib.h>



/**
 *  resize png use file
 *
 *  @param src_filename  input  jpg file name
 *  @param des_filename  output jpg file name
 *  @param size    max width-height
 *
 *  @return 0 sucess or -1 if faild.
 */
int aspire_mao_resize_png_file(const char* src_filename,
							   const char* des_filename,
							   int size)
{
	if (size == 0)
	{
		return -1;
	}
	
	int src_width = 0;
	int src_height = 0;
	int pixel_bytes = 3;
	
	//read to image buffer
	unsigned char* rgb = aspire_mao_png_read_file(src_filename, &src_width, &src_height, &pixel_bytes, 0);
	
	if (!rgb || !src_width || !src_height)
		return -1;
	
	//calc aspect ratio
	float ratio_w = size / (float)src_width;
	float ratio_h = size / (float)src_height;
	float ratio = ratio_w < ratio_h ? ratio_w : ratio_h;
	
	int width = ratio * src_width;
	int height = ratio * src_height;
	
	//gen resized image buffer
	unsigned char * resized_rgb = aspire_mao_resize_rgb(width, height, rgb, src_width, src_height, pixel_bytes);
	
	//free rgb
	free(rgb);
	
	if (!resized_rgb)
		return -1;
	
	//write to file
	int result = aspire_mao_png_write_file(des_filename, resized_rgb, width, height, pixel_bytes);
	
	//free rgb
	free(resized_rgb);
	
	return result;
}

