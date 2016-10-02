//
//  rwfile.h
//  mjpeg
//
//  Created by mao on 02/10/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#ifndef rwfile_h
#define rwfile_h

#include <stdio.h>

/**
 *read file to buffer
 */
unsigned char* read_file(unsigned int *size,
						 const char* filename);

/**
 *write buffer to file
 */
void write_file(unsigned char *buf,
				unsigned int buf_size,
				const char* filename);

#endif /* rwfile_h */
