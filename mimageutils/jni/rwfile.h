/***********************************************************
 //  rwfile.c
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

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
