//
//  resizepng.h
//  mjpeg
//
//  Created by Mao on 28/09/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#ifndef resizepng_h
#define resizepng_h

#include <stdio.h>

/**
 *  resize png use file
 *
 *  @param src_filename  input  jpg file name
 *  @param des_filename  output jpg file name
 *  @param size    max width-height
 *
 *  @return 0 sucess or -1 if faild.
 */
int aspire_mao_resize_png_file(const char* src_filename,
							   const char* des_filename,
							   int size);

#endif /* resizepng_h */
