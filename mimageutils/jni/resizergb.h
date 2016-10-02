//
//  resizergb.h
//  mjpeg
//
//  Created by Mao on 28/09/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#ifndef resizergb_h
#define resizergb_h

#include <stdio.h>

/**
 *  resize rgb data
 *
 *  @param dest_width  dest width
 *  @param dest_height dest height
 *  @param rgb         input rgb data
 *  @param src_width   src width
 *  @param src_height  src height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_resize_rgb(int dest_width,
									 int dest_height,
									 unsigned char *rgb,
									 int src_width,
									 int src_height,
									 int pixel_bytes);

unsigned char* aspire_mao_rgb_flip_horizontal(unsigned char *rgb, int width, int height, int pixel_bytes);

unsigned char* aspire_mao_rgb_flip_vertical(unsigned char *rgb, int width, int height, int pixel_bytes);

#endif /* resizergb_h */
