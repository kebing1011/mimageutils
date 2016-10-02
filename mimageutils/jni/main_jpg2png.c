//
//  main.c
//  MaoJPEG
//
//  Created by mao on 6/25/16.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "jpg2png.h"

void showUsage()
{
	printf("Usage:\n");
	printf("      jpg2png [jpg_file] [png_file]\n");
}


int main(int argc, const char * argv[]) {
	if (argc < 3)
	{
		showUsage();
		return 0;
	}

    const char *arg_input  = argv[1];
    const char *arg_output = argv[2];

	if (aspire_mao_jpg_to_png_file(arg_output, arg_input))
		printf("resize png faild.\n");
	
	
	return 0;
}


