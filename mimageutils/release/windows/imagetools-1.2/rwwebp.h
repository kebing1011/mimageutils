//
//  rwwebp.h
//  mimageutils
//
//  Created by Mao on 19/07/2017.
//  Copyright © 2017 Maokebing. All rights reserved.
//

#ifndef rwwebp_h
#define rwwebp_h

#include <stdio.h>



/**
 *  check is webp
 *
 *  @param img_data img data
 *  @param img_data_size img data size
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_webp(const unsigned char* img_data, unsigned int img_data_size);



/**
 *  check is webp
 *
 *  @param file_name image file name
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_webp_file(const char* file_name);


/**
 *  read webp to rgb data
 *
 *  @param webp_data      input jpg data
 *  @param webp_data_size size of input webp data
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *  @param skip_alpha        skip alpha channel
 *  @param pixel_bytes        3 or 4 rgba
 *
 *  @return rgb data or NULL if error.
 */
unsigned char* aspire_mao_webp_read(const unsigned char* webp_data,
									unsigned int webp_data_size,
									int* width,
									int* height,
									int* pixel_bytes,
									int skip_alpha);


/**
 *  read webp file to rgb data
 *
 *  @param webp_data      input jpg data
 *  @param webp_data_size size of input webp data
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *  @param skip_alpha        skip alpha channel
 *  @param pixel_bytes        3 or 4 rgba
 *
 *  @return rgb data or NULL if error.
 */
unsigned char* aspire_mao_webp_read_file(const char* webp_file_name,
										 int* width,
										 int* height,
										 int* pixel_bytes,
										 int skip_alpha);

/**
 *  write rgb data to webp data
 *
 *  @param webp_data_size output size of webp data
 *  @param rgb_data      input rgb buffer
 *  @param quality       image quality 1~100 advice 75
 *  @param width         input rgb width
 *  @param height        input rgb height
 *
 *  @return output jpg buffer or NULL if error
 */
unsigned char* aspire_mao_webp_write (unsigned int* webp_data_size,
									  const unsigned char* rgb_data,
									  int width,
									  int height,
									  int pixel_bytes,
									  int quality);


/**
 *  write rgb data to webp file
 *
 *  @param webp_file_name filename
 *  @param rgb_data      input rgb buffer
 *  @param quality       image quality 1~100 advice 75
 *  @param width         input rgb width
 *  @param height        input rgb height
 *
 *  @return output jpg buffer or NULL if error
 */
int aspire_mao_webp_write_file (const char* webp_file_name,
								const unsigned char* rgb_data,
								int width,
								int height,
								int pixel_bytes,
								int quality);

#endif /* rwwebp_h */
