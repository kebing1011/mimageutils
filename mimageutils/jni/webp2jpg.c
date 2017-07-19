//
//  webp2jpg.c
//  mimageutils
//
//  Created by Mao on 19/07/2017.
//  Copyright © 2017 Maokebing. All rights reserved.
//

#include "webp2jpg.h"
#include "rwwebp.h"
#include "rwjpg.h"
#include <stdlib.h>


/**
 *  convert webp to jpg data
 *
 *  @param jpg_data_size  output webp buffer
 *  @param webp_data      input webp buffer
 *  @param webp_data_size size of webp buffer
 *  @param quality        image quality 1~100 advice 75
 *
 *  @return 0 sucess or -1 if faild
 */
unsigned char* aspire_mao_webp_to_jpg(unsigned int* jpg_data_size,
									  const unsigned char* webp_data,
									  unsigned int webp_data_size,
									  int quality)
{
	if (jpg_data_size == NULL
		|| webp_data_size == 0
		|| webp_data == NULL
		|| quality > 100
		|| quality < 0)
		return NULL;
	
	if (!aspire_mao_image_is_webp(webp_data, webp_data_size))
		return NULL;
	
	int width = 0;
	int height = 0;
	int pixel_bytes = 3;
	unsigned char* rgb = aspire_mao_webp_read(webp_data, webp_data_size, &width, &height, &pixel_bytes, 1);
	
	if (rgb == NULL)
		return NULL;
	
	unsigned char* jpg_data = aspire_mao_jpg_write(jpg_data_size, rgb, width, height, quality);
	free(rgb);
	
	return jpg_data;
}
