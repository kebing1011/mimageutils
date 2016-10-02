//
//  main.c
//  MaoJPEG
//
//  Created by mao on 6/25/16.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "png2jpg.h"

void showUsage()
{
	printf("Usage:\n");
	printf("      png2jpg [png_file] [jpg_file]\n");
}


int main(int argc, const char * argv[]) {
	if (argc < 3)
	{
		showUsage();
		return 0;
	}

    const char *arg_input  = argv[1];
    const char *arg_output = argv[2];

	if (aspire_mao_png_to_jpg_file(arg_input, arg_output, 100))
		printf("resize png faild.\n");
	
	
	return 0;
}


