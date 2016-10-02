/***********************************************************
 //  resize.h
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

#ifndef resize_h
#define resize_h

#include <stdio.h>
#include <stdlib.h>

/**
 *  resize image data (only PNG & JPG supported)
 *
 *  @param des_img_data_size size of ouptput img data
 *  @param src_img_data      input img data
 *  @param src_img_data_size size of input img data
 *  @param size          input resized size max height-width
 *  @param quality  quality (1-100) max 100 advice use 75 (JPG-only, PNG No effect)
 *
 *  @return output img data or NULL if error.
 */
unsigned char* aspire_mao_resize_image(unsigned int *des_img_data_size,
									   unsigned char* src_img_data,
									   unsigned int src_img_data_size,
									   int size,
									   int quality);

/**
 *  resize jpeg data
 *
 *  @param des_jpg_data_size  the size of output jpg data
 *  @param src_jpg_data input input jpg data
 *  @param src_jpg_data_size  input the size of input jpg data
 *  @param size    max width-height
 *  @param quality  quality (1-100) max 100 advice use 75
 *
 *  @return output jpg data or NULL if error.
 */
unsigned char* aspire_mao_resize_jpg(unsigned int *des_jpg_data_size,
									 unsigned char* src_jpg_data,
									 unsigned int src_jpg_data_size,
									 int size,
									 int quality);



/**
 *  resize png data
 *
 *  @param des_png_data_size size of ouptput png data
 *  @param src_png_data      input png data
 *  @param src_png_data_size size of input png data
 *  @param size          input resized size max height-width
 *
 *  @return output png data or NULL if error.
 */
unsigned char* aspire_mao_resize_png(unsigned int *des_png_data_size,
									 unsigned char* src_png_data,
									 unsigned int src_png_data_size,
									 int size);



/**
 *  convert png to jpg
 *
 *  @param des_jpg_ata_size size of ouptput jpg data
 *  @param src_png_data      input png data
 *  @param src_png_data_size size of input png data
 *  @param quality       input quality for jpg (1~100) 75 as well
 *
 *  @return output jpg data or NULL if error.
 */
unsigned char* aspire_mao_png_to_jpg(unsigned int *des_jpg_data_size,
									 unsigned char* src_png_data,
									 unsigned int src_png_data_size,
									 int quality);


/**
 *  convert jpg to png
 *
 *  @param des_png_data_size  size of ouptput jpg data
 *  @param src_jpg_data      input png data
 *  @param src_jpg_data_size size of input png data
 *
 *  @return output png data or NULL if error.
 */
unsigned char* aspire_mao_jpg_to_png(unsigned int *des_png_data_size,
									 unsigned char* src_jpg_data,
									 unsigned int src_jpg_data_size);


/**
 *  resize & convert png to jpg
 *
 *  @param des_jpg_ata_size size of ouptput jpg data
 *  @param src_png_data      input png data
 *  @param src_png_data_size size of input png data
 *  @param size          input resized size max height-width
 *  @param quality       input quality for jpg (1~100) 75 as well
 *
 *  @return output jpg data or NULL if error.
 */
unsigned char* aspire_mao_resize_png_to_jpg(unsigned int *des_jpg_data_size,
											unsigned char* src_png_data,
											unsigned int src_png_data_size,
											int size,
											int quality);


/**
 *  resize & convert jpg to png
 *
 *  @param des_png_data_size  size of ouptput jpg data
 *  @param src_jpg_data      input png data
 *  @param src_jpg_data_size size of input png data
 *  @param size              input resized size max height-width
 *
 *  @return output png data or NULL if error.
 */
unsigned char* aspire_mao_resize_jpg_to_png(unsigned int *des_png_data_size,
											unsigned char* src_jpg_data,
											unsigned int src_jpg_data_size,
											int size);


/**
 *  check is png
 *
 *  @param img_data img data
 *  @param img_data_size img data size
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_png(unsigned char* img_data,
							unsigned int img_data_size);

/**
 *  check is jpg
 *
 *  @param img_data img data
 *  @param img_data_size img data size
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_jpg(unsigned char* img_data,
							unsigned int img_data_size);



#endif /* resize_h */
