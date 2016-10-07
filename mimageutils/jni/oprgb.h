/***********************************************************
 //  oprgb.h
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

#ifndef oprgb_h
#define oprgb_h

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

/**
 *  flip rgb data
 *
 *  @param rgb     input rgb data
 *  @param width   width
 *  @param height   height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_rgb_flip_horizontal(unsigned char *rgb,
											  int width,
											  int height,
											  int pixel_bytes);
/**
 *  flip rgb data
 *
 *  @param rgb     input rgb data
 *  @param width   width
 *  @param height   height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_rgb_flip_vertical(unsigned char *rgb,
											int width,
											int height,
											int pixel_bytes);


/**
 *  rotate rgb data
 *
 *  @param rgb     input rgb data
 *  @param width   width
 *  @param height   height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_rgb_rotate_right(unsigned char *rgb,
										   int width,
										   int height,
										   int pixel_bytes);

/**
 *  rotate rgb data
 *
 *  @param rgb     input rgb data
 *  @param width   width
 *  @param width   height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_rgb_rotate_left(unsigned char *rgb,
										  int width,
										  int height,
										  int pixel_bytes);



/**
 *  create rgb data
 *
 *  @param rgb     input rgba eg. ff00ffff RGBA
 *  @param width   width
 *  @param width   height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_rgb_create(char* rgba_str,
									 int width,
									 int height,
									 int skip_alpha);

#endif /* resizergb_h */
