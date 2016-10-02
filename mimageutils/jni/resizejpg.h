//
//  resizejpg.h
//  mjpeg
//
//  Created by Mao on 28/09/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#ifndef resizejpg_h
#define resizejpg_h

#include <stdio.h>

/**
 *  resize jpg use file
 *
 *  @param src_filename  input  jpg file name
 *  @param des_filename  output jpg file name
 *  @param size    max width-height
 *  @param quality  quality (1-100) max 100 advice use 75
 *
 *  @return 0 sucess or -1 if faild.
 */
int aspire_mao_resize_jpg_file(const char* src_filename,
							   const char* des_filename,
							   int size,
							   int quality);

#endif /* resizejpg_h */
