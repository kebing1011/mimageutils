//
//  rwpng.h
//  mjpeg
//
//  Created by mao on 02/10/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#ifndef rwpng_h
#define rwpng_h

#include <stdio.h>

/**
 *  read png to rgb data
 *
 *  @param png_data      input png buffer
 *  @param png_data_size size of input png data
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *  @param pixel_bytes   bytes per pixel 3:RGB 4:RGBA
 *  @param skip_alpha    skip alpha
 *
 *  @return rgb buffer
 */
unsigned char* aspire_mao_png_read(const unsigned char* png_data,
								   const unsigned int png_data_size,
								   int* width,
								   int* height,
								   int* pixel_bytes,
								   int skip_alpha);

/**
 *  write rgb to png data
 *
 *  @param png_data_size size of output png data
 *  @param rgb_data      input rgb data
 *  @param width         width of rgb data
 *  @param height        height of rgb data
 *  @param pixel_bytes   bytes per pixel 3:RBG 4:RGBA
 *
 *  @return output png data or NULL if error.
 */
unsigned char* aspire_mao_png_write(unsigned int *png_data_size,
									unsigned char* rgb_data,
									int width,
									int height,
									int pixel_bytes);

/**
 *  read png file to rgb data
 *
 *  @param png_file_name input png file name
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *  @param pixel_bytes   bytes per pixel 3:RGB 4:RGBA
 *  @param skip_alpha    skip alpha
 *
 *  @return rgb buffer
 */
unsigned char* aspire_mao_png_read_file(const char* png_file_name,
										int* width,
										int* height,
										int* pixel_bytes,
										int skip_alpha);

/**
 *  write rgb to png file
 *
 *  @param png_file_name file name of output png data
 *  @param rgb_data      input rgb data
 *  @param width         width of rgb data
 *  @param height        height of rgb data
 *  @param pixel_bytes   bytes per pixel 3:RBG 4:RGBA
 *
 *  @return 0 sucess or -1 if error.
 */
int aspire_mao_png_write_file(const char* png_file_name,
							  unsigned char* rgb_data,
							  int width,
							  int height,
							  int pixel_bytes);

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
 *  check is png
 *
 *  @param file_name image file name
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_png_file(const char* file_name);


#endif /* rwpng_h */
