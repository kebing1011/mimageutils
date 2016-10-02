//
//  main.c
//  MaoJPEG
//
//  Created by mao on 6/25/16.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "rwjpg.h"
#include "rwpng.h"
#include "resizergb.h"
#include <stdio.h>
#include <string.h>


void showUsage()
{
	printf("Usage:\n");
	printf("      imgtl [-qzo] [file ...]\n");
	printf("      -o [output] output image file use format with .jpg or .png\n");
	printf("      -q [quality] for quality 1~100 eg. 75\n");
	printf("      -z [MaxWH] resize aspect to size\n");
	printf("      -s [scale] resize aspect to scale\n");
	printf("      -w [width] resize to width\n");
	printf("      -h [height] resize to height\n");
	printf("      -f [flip] h: horizontal flip or v: vertical flip\n");

}

const char *short_opts = "o:z:q:w:h:s:f:";

int main(int argc, char *argv[])
{
	char opt = 0;
	char *arg_output   = NULL;
	char *arg_size     = NULL;
	char *arg_quality  = NULL;
	char *arg_width    = NULL;
	char *arg_height   = NULL;
	char *arg_scale    = NULL;
	char *arg_flip     = NULL;
	
	
	while((opt = getopt(argc, argv, short_opts))!= -1)
	{
		switch(opt)
		{
			case 'o':
				arg_output = optarg;
				break;
			case 'z':
				arg_size = optarg;
				break;
			case 'q':
				arg_quality = optarg;
				break;
			case 'w':
				arg_width = optarg;
				break;
			case 'h':
				arg_height = optarg;
				break;
			case 's':
				arg_scale = optarg;
				break;
			case 'f':
				arg_flip = optarg;
				break;
		}
	}
	
	//no any arg
	if (arg_output == NULL
		&& arg_size == NULL
		&& arg_quality == NULL
		&& arg_scale == NULL
		&& arg_width == NULL
		&& arg_height == NULL)
	{
		showUsage();
		return 0;
	}
	
	//check confilc arg
	int arg_confilc = 0;
	if (arg_size != NULL)
		arg_confilc++;
	if (arg_width != NULL
		|| arg_height != NULL)
		arg_confilc++;
	if (arg_scale != NULL)
		arg_confilc++;
	
	if (arg_confilc > 1)
	{
		printf("use para conflict with [-s ], [-w or -h], [-z]\n");
		return 0;
	}
	
	//no file arg
	if (optind == argc)
	{
		showUsage();
		return 0;
	}
	
	
	int idx = optind;
	
	//process now.
	while (idx < argc){
		char* img_input  = argv[idx++];
		char* img_output = img_input;
		
		int width = 0;
		int height = 0;
		int pixel_bytes = 3;

		//read to image buffer
		unsigned char* rgb = NULL;
		int to_flag = 0x0;
		int to_jpg = 0x1;
		int to_png = 0x2;
		
		//output image check.
		if (arg_output != NULL)
		{
			//out put file
			img_output = arg_output;
			
			if (strstr(arg_output, "jpg") != NULL
				|| strstr(arg_output, "jpeg") != NULL
				|| strstr(arg_output, "JPG") != NULL
				|| strstr(arg_output, "JPEG") != NULL)
			{
				to_flag = to_jpg;
			}
			else if (strstr(arg_output, "PNG") != NULL
					 || strstr(arg_output, "png") != NULL)
			{
				to_flag = to_png;
			}
			else
			{
				to_flag = 0;
				img_output = NULL;
				
				printf("output file format need use .jpg or .png.\n");
				
				return 0;
			}
		}
		
		//process jpg
		if (aspire_mao_image_is_jpg_file(img_input))
		{
			if (to_flag == 0x0)
				to_flag = to_jpg;
			rgb = aspire_mao_jpg_read_file(img_input, &width, &height);
		}
		else if(aspire_mao_image_is_png_file(img_input))
		{
			if (to_flag == 0x0)
				to_flag = to_png;
			
			//png 2 jpg skip alpha
			if (to_flag == to_jpg)
				rgb = aspire_mao_png_read_file(img_input, &width, &height, &pixel_bytes, 1);
			else
				rgb = aspire_mao_png_read_file(img_input, &width, &height, &pixel_bytes, 0);
		}
		else
		{
			printf("%s is not a support image file.\n", img_input);
			continue;
		}
		
		//check rgb
		if (!rgb || !width || !height)
		{
			printf("read image file faild.\n");
			continue;
		}
		
		//resize check
		if (arg_size!= NULL)
		{
			int size = atoi(arg_size);
			
			//back for use
			float src_width = width;
			float src_height = height;
			
			//calc aspect ratio
			float ratio_w = size / (float)src_width;
			float ratio_h = size / (float)src_height;
			float ratio = ratio_w < ratio_h ? ratio_w : ratio_h;
			
			
			width = ratio * src_width;
			height = ratio * src_height;
			
			//gen resized image buffer
			unsigned char* src_rgb = rgb;
			
			//gen new rgb
			rgb = aspire_mao_resize_rgb(width, height, src_rgb, src_width, src_height, pixel_bytes);
			
			//free src_rgb
			free(src_rgb);
		}
		
		//resize for width or height
		if (arg_width != NULL
			|| arg_height != NULL)
		{
			//back for use
			float src_width = width;
			float src_height = height;
			
			//new width or height
			if (arg_width != NULL)
				width = atoi(arg_width);
			
			if (arg_height != NULL)
				height = atoi(arg_height);
			
			//gen resized image buffer
			unsigned char* src_rgb = rgb;
			
			//gen new rgb
			rgb = aspire_mao_resize_rgb(width, height, src_rgb, src_width, src_height, pixel_bytes);
			
			//free src_rgb
			free(src_rgb);
		}
		
		//resize with scale
		if (arg_scale != NULL)
		{
			float scale = atof(arg_scale);
			if (scale < 0
				|| scale > 2.0)
			{
				free(rgb);
				printf("para scale error with %s\n", arg_scale);
				return 0;
			}
			
			//back for use
			float src_width = width;
			float src_height = height;
			
			//new width & height
			width = scale * src_width;
			height = scale * src_height;
			
			//gen resized image buffer
			unsigned char* src_rgb = rgb;
			
			//gen new rgb
			rgb = aspire_mao_resize_rgb(width, height, src_rgb, src_width, src_height, pixel_bytes);
			
			//free src_rgb
			free(src_rgb);
		}
		
		
		//flip image
		if (arg_flip != NULL)
		{
			if (strcmp(arg_flip, "v") == 0)
			{
				//gen resized image buffer
				unsigned char* src_rgb = rgb;
				
				//gen new rgb
				rgb = aspire_mao_rgb_flip_vertical(src_rgb, width, height, pixel_bytes);
				
				//free src_rgb
				free(src_rgb);
			}
			else if (strcmp(arg_flip, "h") == 0)
			{
				//gen resized image buffer
				unsigned char* src_rgb = rgb;
				
				//gen new rgb
				rgb = aspire_mao_rgb_flip_horizontal(src_rgb, width, height, pixel_bytes);
				
				//free src_rgb
				free(src_rgb);
			}
		}
		
		//write image to file
		if (to_flag == to_png)
		{
			if (aspire_mao_png_write_file(img_output, rgb, width, height, pixel_bytes))
			{
				printf("write image file faild.\n");
			}
		}
		else if (to_flag == to_jpg)
		{
			//only jpg need quality
			int quality = 100;
			if (arg_quality != NULL)
			{
				quality = atoi(arg_quality);
			}
			
			if (aspire_mao_jpg_write_file(img_output, rgb, width, height, quality))
			{
				printf("write image file faild.\n");
			}
		}
		else
		{
			printf("output file format need use .jpg or .png.\n");
		}
		
		//free rgb final
		free(rgb);
	}
	
	return 0;
}
