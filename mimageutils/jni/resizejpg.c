//
//  resizejpg.c
//  mjpeg
//
//  Created by Mao on 28/09/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include "resizejpg.h"
#include <stdlib.h>
#include <string.h>
#include "resizergb.h"
#include "rwjpg.h"


/**
 *  resize jpg use file
 *
 *  @param src_filename  input  jpg file name
 *  @param des_filename  output jpg file name
 *  @param size    max width-height
 *  @param quality  quality (1-100) max 100 advice use 75
 *
 *  @return 0 sucess or -1 if faild.
 */
int aspire_mao_resize_jpg_file(const char* src_filename,
							   const char* des_filename,
							   int size,
							   int quality)
{
	if (size == 0)
	{
		return -1;
	}
	
	int src_width = 0;
	int src_height = 0;
	
	//read to image buffer
	unsigned char* rgb = aspire_mao_jpg_read_file(src_filename, &src_width, &src_height);
	
	if (!rgb || !src_width || !src_height)
		return -1;
	
	//calc aspect ratio
	float ratio_w = size / (float)src_width;
	float ratio_h = size / (float)src_height;
	float ratio = ratio_w < ratio_h ? ratio_w : ratio_h;
	
	int width = ratio * src_width;
	int height = ratio * src_height;
	
	//gen resized image buffer
	unsigned char * resized_rgb = aspire_mao_resize_rgb(width, height, rgb, src_width, src_height, 3);
	
	//free rgb
	free(rgb);
	
	if (!resized_rgb)
		return -1;
	
	//write to file
	int result = aspire_mao_jpg_write_file(des_filename, resized_rgb, width, height, quality);
	
	//free rgb
	free(resized_rgb);
	
	return result;
}
