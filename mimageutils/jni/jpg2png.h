//
//  jpg2png.h
//  mjpeg
//
//  Created by Mao on 28/09/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#ifndef jpg2png_h
#define jpg2png_h

#include <stdio.h>

/**
 *  convert jpg to png
 *
 *  @param png_file_name  output png file name
 *  @param jpg_file_name  input jpg file name
 *
 *  @return 0 or -1 if error.
 */
int aspire_mao_jpg_to_png_file(const char* png_file_name,
							   const char* jpg_file_name);

#endif /* jpg2png_h */
