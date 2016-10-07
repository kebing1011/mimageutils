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
#include "oprgb.h"
#include <stdio.h>
#include <string.h>


void showUsage()
{
	printf("Usage:\n");
	printf("      imgmk -o 1.jpg -w 100 -h 100 -r ffffff(RGBA)\n");
	printf("      -o [output] output image file use format with .jpg or .png\n");
	printf("      -w [width] set width\n");
	printf("      -h [height] set height\n");
	printf("      -r [rgb] rgba or rgb eg. ff00ff00, ff00ff\n");
}

const char *short_opts = "o:w:h:r:";

int main(int argc, char *argv[])
{
	char opt = 0;
	char *arg_output   = NULL;
	char *arg_width    = NULL;
	char *arg_height   = NULL;
	char *arg_rgba     = NULL;
	
	while((opt = getopt(argc, argv, short_opts))!= -1)
	{
		switch(opt)
		{
			case 'o':
				arg_output = optarg;
				break;
			case 'w':
				arg_width = optarg;
				break;
			case 'h':
				arg_height = optarg;
				break;
			case 'r':
				arg_rgba = optarg;
				break;
		}
	}
	
	//no any arg
	if (arg_output == NULL
		|| arg_width == NULL
		|| arg_height == NULL)
	{
		showUsage();
		return 0;
	}
	
	int width = atoi(arg_width);
	int height = atoi(arg_height);
	int pixel_bytes = 3;
	
	//read to image buffer
	int to_flag = 0x0;
	int to_jpg = 0x1;
	int to_png = 0x2;
	
	//out put file
	if (strstr(arg_output, "jpg") != NULL
		|| strstr(arg_output, "jpeg") != NULL
		|| strstr(arg_output, "JPG") != NULL
		|| strstr(arg_output, "JPEG") != NULL)
	{
		to_flag = to_jpg;
		
		unsigned char* rgb = aspire_mao_rgb_create(arg_rgba, width, height, 1);
		
		if (aspire_mao_jpg_write_file(arg_output, rgb, width, height, 100))
		{
			printf("write image file faild.\n");
		}
		
		free(rgb);
	}
	else if (strstr(arg_output, "PNG") != NULL
			 || strstr(arg_output, "png") != NULL)
	{
		to_flag = to_png;
		
		unsigned char* rgb = aspire_mao_rgb_create(arg_rgba, width, height, 0);
		
		if (aspire_mao_png_write_file(arg_output, rgb, width, height, pixel_bytes))
		{
			printf("write image file faild.\n");
		}
		
		free(rgb);
	}
	else
	{
		printf("output file format need use .jpg or .png.\n");
		return 0;
	}

	return 0;
}
