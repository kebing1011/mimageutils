/***********************************************************
 //  mimageutils comman line tool
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resize.h"
#include "rwfile.h"

int main(int argc, char *argv[])
{
	unsigned int src_size = 0;
//	unsigned char* img_data = read_file(&src_size, "1.jpg");
	unsigned char* img_data = read_file(&src_size, "1.png");


	
	unsigned int out_size = 0;
	unsigned char* output = aspire_mao_resize_image(&out_size, img_data, src_size, 300, 100);
	free(img_data);
	
	if (output)
	{
//		write_file(output, out_size, "1_1.jpg");
		write_file(output, out_size, "1-1.png");
	}
	
	free(output);
	
	
	return 0;
}
