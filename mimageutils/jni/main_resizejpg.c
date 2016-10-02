//
//  main.c
//  MaoJPEG
//
//  Created by mao on 6/25/16.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "resizejpg.h"
#include "resizepng.h"

void showUsage()
{
	printf("Usage:\n");
	printf("resizejpg: -f [file] input image file\n");
	printf("           -o [file] output image file\n");
	printf("           -q [quality] for quality 1~100 eg. 75\n");
	printf("           -z [MaxWH] for resize into size\n");
}

const char *short_opts = "f:o:z:q:";

int main(int argc, const char * argv[]) {
	if (argc <= 1)
	{
		showUsage();
		return 0;
	}

	char opt = 0;
    char *arg_input  = NULL;
    char *arg_output = NULL;
    char *arg_size   = NULL;
    char *arg_quality = NULL;
	while ((opt = getopt(argc, (const void *)argv, short_opts)) != -1) {
		switch (opt) {
			case 'f':
				arg_input = optarg;
				break;
			case 'o':
				arg_output = optarg;
				break;
			case 'z':
				arg_size = optarg;
				break;
			case 'q':
				arg_quality = optarg;
				break;
			default:
				break;
		}
	}

	if (arg_input == NULL
		|| arg_output == NULL
		|| arg_size == NULL)
	{
		showUsage();
		return 0;
	}
	
	int size = atoi(arg_size);
	int quality = 100;
	if (arg_quality) {
		quality = atoi(arg_quality);
	}
	
	if (aspire_mao_resize_jpg_file(arg_input, arg_output, size, quality))
		printf("resize jpeg faild.\n");
	
	
	return 0;
}


