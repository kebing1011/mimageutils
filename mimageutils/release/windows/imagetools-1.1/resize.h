/***********************************************************
 //  resize.h
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

#ifndef resize_h
#define resize_h

#include <stdio.h>

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
									   int quality);

#endif /* resize_h */
