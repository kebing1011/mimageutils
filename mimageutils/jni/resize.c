/***********************************************************
 //  resize.h
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

#include <stdlib.h>
#include "resize.h"
#include "rwjpg.h"
#include "rwpng.h"
#include "rwwebp.h"
#include "oprgb.h"


/**
 *  resize image data(only for png & jpeg)
 *
 *  @param dest_image_size size of resized image data
 *  @param src_image_data  data of input image
 *  @param src_image_data  size of input image data
 *  @param resize use image max-WH
 *  @param quality (1-100 jpeg only)
 *
 *  @return resized image data  or NULL if error.
 */
unsigned char* aspire_mao_resize_image(unsigned int* dest_image_size,
									   unsigned char* src_image_data,
									   unsigned int src_image_size,
									   unsigned int size,
									   int quality)
{
	if (dest_image_size == NULL
		|| src_image_data == NULL
		|| src_image_size == 0
		|| size == 0
		|| quality <= 0
		|| quality > 100)
		return NULL;
	
	//set some vals
	int width       = 0;
	int height      = 0;
	int pixel_bytes = 3;
	int is_png      = 0;
	int is_jpg      = 0;
	int is_webp     = 0;
	unsigned char* rgb = NULL;
	
	//read image to rgb
	if (aspire_mao_image_is_jpg(src_image_data, src_image_size))
	{
		is_jpg = 1;
		rgb = aspire_mao_jpg_read(src_image_data, src_image_size, &width, &height);
	}
	else if (aspire_mao_image_is_png(src_image_data, src_image_size))
	{
		is_png = 1;
		rgb = aspire_mao_png_read(src_image_data, src_image_size, &width, &height, &pixel_bytes, 0);
	}
	else if (aspire_mao_image_is_webp(src_image_data, src_image_size)){
		is_webp = 1;
		rgb = aspire_mao_webp_read(src_image_data, src_image_size, &width, &height, &pixel_bytes, 0);
	}else {
		return NULL;
	}
	
	//check output
	if (rgb == NULL
		|| width == 0
		|| height == 0)
		return NULL;
	
	//calc aspect ratio
	float ratio_w = size / (float)width;
	float ratio_h = size / (float)height;
	float ratio = ratio_w < ratio_h ? ratio_w : ratio_h;
	
	//dest size
	int dest_width = ratio * width;
	int dest_height = ratio * height;
	
	//resize rgb now.
	unsigned char* resize_rgb = aspire_mao_resize_rgb(dest_width, dest_height, rgb, width, height, pixel_bytes);
	
	//free src_rgb
	free(rgb);
	
	
	//write to image data
	unsigned char* output = NULL;
	if (is_jpg)
	{
		output = aspire_mao_jpg_write(dest_image_size, resize_rgb, dest_width, dest_height, quality);
	}
	else if (is_png)
	{
		output = aspire_mao_png_write(dest_image_size, resize_rgb, dest_width, dest_height, pixel_bytes);
	}else if (is_webp) {
		output = aspire_mao_webp_write(dest_image_size, resize_rgb, dest_width, dest_height, pixel_bytes, quality);
	}
	
	//free resize_rgb
	free(resize_rgb);
	
	return output;
}
