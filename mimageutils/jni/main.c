/***********************************************************
 //  mimageutils comman line tool
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "rwjpg.h"
#include "rwpng.h"
#include "rwfile.h"
#include "oprgb.h"
#include <stdio.h>
#include <string.h>


void showUsage()
{
	printf("Usage:\n");
	printf("      imgtl [-oqzswhfr] [file ...]\n");
	printf("      -o [output] output image file use format with .jpg or .png\n");
	printf("      -q [quality] for quality 1~100 eg. 75\n");
	printf("      -z [MaxWH] resize aspect to size\n");
	printf("      -s [scale] resize aspect to scale\n");
	printf("      -w [width] resize to width\n");
	printf("      -h [height] resize to height\n");
	printf("      -f [flip] h: horizontal flip or v: vertical flip\n");
	printf("      -r [rotate] l: left rotate or r: right rotate\n");
}

const char *short_opts = "o:z:q:w:h:s:f:r:";

int main(int argc, char *argv[])
{
	unsigned int src_size = 0;
	unsigned char* img_data = read_file(&src_size, "1.jpg");
	
	unsigned char* rgb = NULL;
	int width = 0;
	int height = 0;
	int pixel_bytes = 3;
	int is_png = 0;
	int is_jpg = 0;
	int quality = 100;
	
	int size = 300;
	
	if (aspire_mao_image_is_jpg(img_data, src_size))
	{
		rgb = aspire_mao_jpg_read(img_data, src_size, &width, &height);
		is_jpg = 1;
	}
	else if(aspire_mao_image_is_png(img_data, src_size))
	{
		rgb = aspire_mao_png_read(img_data, src_size, &width, &height, &pixel_bytes, 0);
		is_png = 1;
	}
	
	free(img_data);
	
	
	if (rgb == NULL
		|| width == 0
		|| height == 0)
		return 0;
	
	
	//calc aspect ratio
	float ratio_w = size / (float)width;
	float ratio_h = size / (float)height;
	float ratio = ratio_w < ratio_h ? ratio_w : ratio_h;
	
	//dest size
	int dest_width = ratio * width;
	int dest_height = ratio * height;
	
	//gen new rgb
	unsigned char* resize_rgb = aspire_mao_resize_rgb(dest_width, dest_height, rgb, width, height, pixel_bytes);
	
	//free src_rgb
	free(rgb);
	
	
	unsigned int out_size = 0;
	unsigned char* output = NULL;
	if (is_jpg) {
		output = aspire_mao_jpg_write(&out_size, resize_rgb, dest_width, dest_height, quality);
	}else if (is_png) {
		output = aspire_mao_png_write(&out_size, resize_rgb, dest_width, dest_height, pixel_bytes);
	}
		
	if (output){
		write_file(output, out_size, "333.jpg");
	}
	
	free(output);
	
	
	return 0;
}
