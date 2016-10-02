//
//  png2jpg.c
//  mjpeg
//
//  Created by Mao on 28/09/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include <stdlib.h>
#include "png2jpg.h"
#include "rwjpg.h"
#include "rwpng.h"


/**
 *  convert png to jpg
 *
 *  @param png_file_name  input png file name
 *  @param jpg_file_name  output jpg file name
 *
 *  @return 0 or -1 if error.
 */
int aspire_mao_png_to_jpg_file(const char* png_file_name, const char* jpg_file_name, int quality)
{
	int src_width = 0;
	int src_height = 0;
	int pixel_bytes = 3;
	
	//read to image buffer
	unsigned char* rgb = aspire_mao_png_read_file(png_file_name, &src_width, &src_height, &pixel_bytes, 1);
	
	if (!rgb || !src_width || !src_height)
		return -1;
	
	//write to file
	int result = aspire_mao_jpg_write_file(jpg_file_name, rgb, src_width, src_height, quality);
	
	//free rgb
	free(rgb);
	
	return result;
}


