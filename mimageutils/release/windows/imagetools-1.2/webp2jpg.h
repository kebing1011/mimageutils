//
//  webp2jpg.h
//  mimageutils
//
//  Created by Mao on 19/07/2017.
//  Copyright © 2017 Maokebing. All rights reserved.
//

#ifndef webp2jpg_h
#define webp2jpg_h

#include <stdio.h>


/**
 *  convert webp to jpg data
 *
 *  @param jpg_data_size  output webp buffer
 *  @param webp_data      input webp buffer
 *  @param webp_data_size size of webp buffer
 *  @param quality        image quality 1~100 advice 75
 *
 *  @return 0 sucess or -1 if faild
 */
unsigned char* aspire_mao_webp_to_jpg(unsigned int* jpg_data_size,
									  const unsigned char* webp_data,
									  unsigned int webp_data_size,
									  int quality);


#endif /* webp2jpg_h */
