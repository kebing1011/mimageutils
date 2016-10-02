//
//  png2jpg.h
//  mjpeg
//
//  Created by Mao on 28/09/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#ifndef png2jpg_h
#define png2jpg_h

#include <stdio.h>

/**
 *  convert png to jpg
 *
 *  @param png_file_name  input png file name
 *  @param jpg_file_name  output jpg file name
 *
 *  @return 0 or -1 if error.
 */
int aspire_mao_png_to_jpg_file(const char* png_file_name,
							   const char* jpg_file_name,
							   int quality);

#endif /* png2jpg_h */
