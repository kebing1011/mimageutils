//
//  main.c
//  MaoJPEG
//
//  Created by mao on 6/25/16.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "resize.h"
#include <unistd.h>
#include <getopt.h>
#include <setjmp.h>
#include "resizejpg.h"


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


void showUsage()
{
	printf("Usage:\n");
	printf("resizejpg: -f [file] input image file\n");
	printf("           -o [file] output image file\n");
	printf("           -z [MaxWH] for resize into size\n");
}

const char *short_opts = "o:z:c:f:";

int main(int argc, const char * argv[]) {
//	unsigned input_buf_len = 0;
//	unsigned char* inputBuf = read_file(&input_buf_len, "1.jpg");
//	unsigned output_buf_len = 0;
//	
//	unsigned char* fff = malloc(input_buf_len);
//	memcpy(fff, inputBuf, input_buf_len);
//	free(inputBuf);
//	
//	printf("%p\n", fff);
//	printf("%p\n", inputBuf);
//
//	unsigned char* outputBuf = aspire_mao_resize_image(&output_buf_len, fff, input_buf_len, 300, 100);
//	write_file(outputBuf, output_buf_len, "100.jpg");
//	free(fff);
//	free(outputBuf);
//
//	
//	return 0;
//
//	if (argc <= 1)
//	{
//		showUsage();
//		return 0;
//	}
//
//	char opt = 0;
//    char *arg_input   = NULL;
//    char *arg_output  = NULL;
//    char *arg_size    = NULL;
//    char *arg_format  = NULL;
//	while ((opt = getopt(argc, (const void *)argv, short_opts)) != -1) {
//		switch (opt) {
//			case 'f':
//				arg_input = optarg;
//				break;
//			case 'o':
//				arg_output = optarg;
//				break;
//			case 'z':
//				arg_size = optarg;
//				break;
//			case 'c':
//				arg_format = optarg;
//				break;
//			default:
//				break;
//		}
//	}
//
//	if (arg_input == NULL
//		|| arg_output == NULL)
//	{
//		showUsage();
//		return 0;
//	}
//	
//	
//	unsigned input_buf_len = 0;
//	unsigned char* tempBuf = read_file(&input_buf_len, arg_input);
//	unsigned char* inputBuf = malloc(input_buf_len);
//	memcpy(inputBuf, tempBuf, input_buf_len);
//	free(tempBuf);
//	
//	unsigned output_buf_len = 0;
//	unsigned char* outputBuf = NULL;
//
//	if (inputBuf == NULL){
//		printf("%s is not exsit.\n", arg_input);
//		return 0;
//	}
//	
//	//resize
//	if (arg_size != NULL) {
//		int size = atoi(arg_size);
//		outputBuf = aspire_mao_resize_image(&output_buf_len, inputBuf, input_buf_len, size, 100);
//		if (outputBuf == NULL)
//		{
//			printf("resize faild.\n");
//		}
//		else
//		{
//			write_file(outputBuf, output_buf_len, arg_output);
//		}
//		
//		free(outputBuf);
//		free(inputBuf);
//		return 0;
//	}
//	
//	//convert start
//	if (arg_format != NULL) {
//		
//		//to jpg
//		if (strcmp(arg_format, "jpg") == 0) {
//			//png to jpg
//			if (aspire_mao_image_is_png(inputBuf, input_buf_len)) {
//				outputBuf = aspire_mao_png_to_jpg(&output_buf_len, inputBuf, input_buf_len, 100);
//				if (outputBuf == NULL)
//				{
//					printf("convert faild.\n");
//				}
//				else
//				{
//					write_file(outputBuf, output_buf_len, arg_output);
//				}
//			}
//			else
//			{
//				printf("image input is jpg alredy.\n");
//			}
//		}
//		else if(strcmp(arg_format, "png") == 0)
//		{
//			//jpg to jpg
//			if (aspire_mao_image_is_jpg(inputBuf, input_buf_len))
//			{
//				outputBuf = aspire_mao_jpg_to_png(&output_buf_len, inputBuf, input_buf_len);
//				
//				if (outputBuf == NULL)
//					printf("convert faild.\n");
//				else
//					write_file(outputBuf, output_buf_len, arg_output);
//			}else
//				printf("image input is png alredy.\n");
//
//		}
//		else
//		{
//			printf("unsupport image format: %s\n", arg_format);
//		}
//	}
//	
//
//	
//	if (inputBuf)
//		free(inputBuf);
//	if (outputBuf)
//		free(outputBuf);
	
	aspire_mao_resize_jpg_file("1.jpg", "2.jpg", 500, 100);
	
	
	return 0;
}


