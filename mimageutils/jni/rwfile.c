//
//  rwfile.c
//  mjpeg
//
//  Created by mao on 02/10/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include "rwfile.h"
#include <stdlib.h>


unsigned char* read_file(unsigned int *size, const char* filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
		return NULL;
	
	fseek(file, 0, SEEK_END);
	size_t len = (size_t )ftell(file);
	unsigned char *buf = malloc(len);
	if (buf == NULL)
		return NULL;
	
	rewind(file);
	fread(buf, len, 1, file);
	fclose(file);
	
	if (size)
		*size = (int)len;
	
	return buf;
}


void write_file(unsigned char *buf, unsigned int buf_size, const char* filename)
{
	if (buf == NULL)
		return;
	
	FILE*  pFile = fopen(filename, "w");
	fwrite(buf, buf_size, 1, pFile);
	fflush(pFile);
	fclose(pFile);
}
