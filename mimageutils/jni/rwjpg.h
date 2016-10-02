//
//  rwjpg.h
//  mjpeg
//
//  Created by mao on 02/10/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#ifndef rwjpg_h
#define rwjpg_h

#include <stdio.h>

/**
 *  read jpg to rgb data
 *
 *  @param jpg_data      input jpg data
 *  @param jpg_data_size size of input jpg data
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *
 *  @return rgb data or NULL if error.
 */
unsigned char* aspire_mao_jpg_read(const unsigned char* jpg_data,
								   unsigned int jpg_data_size,
								   int* width,
								   int* height);


/**
 *  write rgb data to jpg data
 *
 *  @param jpg_data_size output size of jpg data
 *  @param rgb_data      input rgb buffer
 *  @param quality       image quality 1~100 advice 75
 *  @param width         input rgb width
 *  @param height        input rgb height
 *
 *  @return output jpg buffer or NULL if error
 */
unsigned char* aspire_mao_jpg_write (unsigned int* jpg_data_size,
									 const unsigned char* rgb_data,
									 int width,
									 int height,
									 int quality);


/**
 *  read jpg file to rgb data
 *
 *  @param jpg_file_name input jpg file name
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *
 *  @return rgb data or NULL if error.
 */
unsigned char* aspire_mao_jpg_read_file(const char* jpg_file_name,
										int* width,
										int* height);

/**
 *  write rgb data to jpg file
 *
 *  @param jpg_file_name output jpg file name
 *  @param rgb_data      input rgb buffer
 *  @param quality       image quality 1~100 advice 75
 *  @param width         input rgb width
 *  @param height        input rgb height
 *
 *  @return 0 sucess or -1 if faild
 */
int aspire_mao_jpg_write_file(const char* jpg_file_name,
							  const unsigned char* rgb_data,
							  int width, int height,
							  int quality);


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

/**
 *  check is jpg
 *
 *  @param file_name image file name
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_jpg_file(const char* file_name);

#endif /* rwjpg_h */
