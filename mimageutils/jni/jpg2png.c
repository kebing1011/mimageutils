//
//  jpg2png.c
//  mjpeg
//
//  Created by Mao on 28/09/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include "jpg2png.h"
#include "rwpng.h"
#include "rwjpg.h"
#include <stdlib.h>

/**
 *  convert jpg to png
 *
 *  @param png_file_name  output png file name
 *  @param jpg_file_name  input jpg file name
 *
 *  @return 0 or -1 if error.
 */
int aspire_mao_jpg_to_png_file(const char* png_file_name, const char* jpg_file_name)
{
	int src_width = 0;
	int src_height = 0;
	int pixel_bytes = 3;
	
	//read to image buffer
	unsigned char* rgb = aspire_mao_jpg_read_file(jpg_file_name, &src_width, &src_height);
	
	if (!rgb || !src_width || !src_height)
		return -1;
	
	//write to file
	int result = aspire_mao_png_write_file(png_file_name, rgb, src_width, src_height, pixel_bytes);
	
	//free rgb
	free(rgb);
	
	return result;
}
