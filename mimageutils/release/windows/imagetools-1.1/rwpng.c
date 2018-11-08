/***********************************************************
 //  rwpng.c
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

#include "rwpng.h"
#include <setjmp.h>
#include <png.h>
#include <stdlib.h>
#include <string.h>

/**
 *  png image source
 */
typedef struct
{
	unsigned char* data;
	int size;
	int offset;
}png_image_source;

/**
 *  png read callback
 *
 *  @param png_ptr ptr
 *  @param data    data
 *  @param length length
 */
static void png_read_callback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_image_source* isource = (png_image_source*)png_get_io_ptr(png_ptr);
	if(isource->offset + length <= isource->size)
	{
		memcpy(data, isource->data+isource->offset, length);
		isource->offset += length;
	}
	else
		png_error(png_ptr, "png_read_callback failed");
}

/**
 *  png write callback
 *
 *  @param png_ptr png_ptr
 *  @param data    data
 *  @param length  length
 */
static void png_write_callback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	png_image_source* isource = (png_image_source*)png_get_io_ptr(png_ptr);
	
	if (isource->size > 0)
		isource->data = realloc(isource->data, isource->size + length);
	else
		isource->data = malloc(length);
	
	memcpy(isource->data + isource->offset, data, length);
	isource->offset += length;
	isource->size += length;
}

/**
 *  png flush callback
 *
 *  @param png_ptr png_ptr
 */
static void png_flush_callback(png_structp png_ptr)
{
	printf("png_flush_callback\n");
}

/**
 *  read png to rgb data
 *
 *  @param png_data      input png buffer
 *  @param png_data_size size of input png data
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *  @param pixel_bytes   bytes per pixel 3:RGB 4:RGBA
 *  @param skip_alpha    skip alpha
 *
 *  @return rgb buffer
 */
unsigned char* aspire_mao_png_read(const unsigned char* png_data, const unsigned int png_data_size, int* width, int* height, int* pixel_bytes, int skip_alpha)
{
	//output para
	unsigned int size = 0;
	unsigned char* output = NULL;
	int png_pixel_bytes = 3;
	
	//read png_ptr
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if(png_ptr == NULL)
		return NULL;
	
	//read info_ptr
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, 0, 0);
		return NULL;
	}
	
	//jmp
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr,0);
	}
	
	//imgae source for read callback
	png_image_source img_source;
	img_source.data = (unsigned char*)png_data;
	img_source.size = png_data_size;
	img_source.offset = 0;
	
	//read callback set.
	png_set_read_fn(png_ptr, &img_source, png_read_callback);
	png_read_png( png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0 );
	int color_type = png_get_color_type( png_ptr, info_ptr );
	int w = png_get_image_width( png_ptr, info_ptr );
	int h = png_get_image_height( png_ptr, info_ptr );
	png_bytep* row_pointers = png_get_rows( png_ptr, info_ptr );
	
	switch( color_type ) {
		case PNG_COLOR_TYPE_RGBA:
		{
			if (!skip_alpha)
			{
				png_pixel_bytes = 4;
				size = w * h * 4;
				int i = 0;
				output = malloc(size);
				//malloc failed.
				if (output == NULL)
					return NULL;
				
				//read to rgb arrays
				for(int y = 0; y < h; ++y) {
					for (int x = 0; x < w; ++x) {
						int z = x * 4;
						output[i++] = row_pointers[y][z++]; // red
						output[i++] = row_pointers[y][z++]; // green
						output[i++] = row_pointers[y][z++]; // blue
						output[i++] = row_pointers[y][z++]; // alpha
					}
				}
			}
			else
			{
				png_pixel_bytes = 3;
				size = w * h * 3;
				int i = 0;
				output = malloc(size);
				//malloc failed.
				if (output == NULL)
					return NULL;
				
				//read to rgb arrays
				for(int y = 0; y < h; ++y) {
					for (int x = 0; x < w; ++x) {
						int z = x * 4;
						output[i++] = row_pointers[y][z++]; // red
						output[i++] = row_pointers[y][z++]; // green
						output[i++] = row_pointers[y][z++]; // blue
						//skip alpha
					}
				}
			}
		}
			break;
			
		case PNG_COLOR_TYPE_RGB:
		{
			png_pixel_bytes = 3;
			size = w * h * 3;
			output = malloc(size);
			//malloc failed.
			if (output == NULL)
				return NULL;
			
			//read to rgb arrays
			int i = 0;
			for(int y = 0; y < h; ++y) {
				for (int x = 0; x < w; ++x) {
					int z = x * 3;
					output[i++] = row_pointers[y][z++]; // red
					output[i++] = row_pointers[y][z++]; // green
					output[i++] = row_pointers[y][z++]; // blue
				}
			}
		}
			break;
		default:
			png_destroy_read_struct( &png_ptr, &info_ptr, 0);
	}
	
	png_destroy_read_struct( &png_ptr, &info_ptr, 0);
	
	//read finish!
	if (width)
		*width = w;
	if (height)
		*height = h;
	if (pixel_bytes)
		*pixel_bytes = png_pixel_bytes;
	
	return output;
}

/**
 *  write rgb to png data
 *
 *  @param png_data_size size of output png data
 *  @param rgb_data      input rgb data
 *  @param width         width of rgb data
 *  @param height        height of rgb data
 *  @param pixel_bytes   bytes per pixel 3:RBG 4:RGBA
 *
 *  @return output png data or NULL if error.
 */
unsigned char* aspire_mao_png_write(unsigned int *png_data_size, unsigned char* rgb_data, int width, int height, int pixel_bytes)
{
	if (!rgb_data
		|| !width
		|| !height
		|| !(pixel_bytes == 3 || pixel_bytes == 4)) //RGB or RGBA
		return NULL;
	
	//create png struct
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
		return NULL;
	
	//create png info
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_write_struct(&png_ptr,  NULL);
		return NULL;
	}
	
	//set jmp
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return NULL;
	}
	
	//imgae source for write callback
	png_image_source img_source;
	img_source.data = NULL;
	img_source.size = 0;
	img_source.offset = 0;
	png_set_write_fn(png_ptr, &img_source, png_write_callback, png_flush_callback);
	
	//set ihdr
	if (pixel_bytes == 3)//RGB
		png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB,
					 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	else //RGBA
		png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGBA,
					 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	
	//write png info
	png_write_info(png_ptr, info_ptr);
	
	//alloc mem
	unsigned char **row_pointers = malloc(height * sizeof(unsigned char *));
	for (int i = 0; i < height; ++i) {
		row_pointers[i] = malloc(width * pixel_bytes);
	}
	
	//assign to row_pointers
	int i = 0;
	if (pixel_bytes == 3)
	{
		for (int k = 0; k < height; k++)
			for (int x = 0; x < width; ++x) {
				int z = x * 3;
				row_pointers[k][z++] = rgb_data[i++];
				row_pointers[k][z++] = rgb_data[i++];
				row_pointers[k][z++] = rgb_data[i++];
			}
	}
	else
	{
		for (int k = 0; k < height; k++)
			for (int x = 0; x < width; ++x) {
				int z = x * 4;
				row_pointers[k][z++] = rgb_data[i++];
				row_pointers[k][z++] = rgb_data[i++];
				row_pointers[k][z++] = rgb_data[i++];
				row_pointers[k][z++] = rgb_data[i++];
			}
	}
	
	//write image
	png_write_image(png_ptr, row_pointers);
	
	//write end
	png_write_end(png_ptr, info_ptr);
	
	//release ptr & info
	png_destroy_write_struct(&png_ptr, &info_ptr);
	
	//relase row_pointers
	for (int i = 0; i < height; ++i) {
		free(row_pointers[i]);
	}
	
	free(row_pointers);
	
	//output png data
	if (png_data_size)
		*png_data_size = img_source.size;
	
	return img_source.data;
}


/**
 *  read png file to rgb data
 *
 *  @param png_file_name input png file name
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *  @param pixel_bytes   bytes per pixel 3:RGB 4:RGBA
 *  @param skip_alpha    skip alpha
 *
 *  @return rgb buffer
 */
unsigned char* aspire_mao_png_read_file(const char* png_file_name, int* width, int* height, int* pixel_bytes, int skip_alpha)
{
	FILE* pFile = fopen(png_file_name, "rb");
	if (pFile == NULL)
		return NULL;
	
	//output para
	unsigned int size = 0;
	unsigned char* output = NULL;
	int png_pixel_bytes = 3;
	
	//read png_ptr
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	if(png_ptr == NULL)
	{
		fclose(pFile);
		return NULL;
	}
	//read info_ptr
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL)
	{
		png_destroy_read_struct(&png_ptr, 0, 0);
		fclose(pFile);
		return NULL;
	}
	
	//jmp
	if(setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr,0);
	}
	
	png_init_io( png_ptr, pFile);
	png_read_png( png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0 );
	int color_type = png_get_color_type( png_ptr, info_ptr );
	int w = png_get_image_width( png_ptr, info_ptr );
	int h = png_get_image_height( png_ptr, info_ptr );
	png_bytep* row_pointers = png_get_rows( png_ptr, info_ptr );
	
	switch( color_type ) {
		case PNG_COLOR_TYPE_RGBA:
		{
			if (!skip_alpha)
			{
				png_pixel_bytes = 4;
				size = w * h * 4;
				int i = 0;
				output = malloc(size);
				//malloc failed.
				if (output == NULL)
				{
					fclose(pFile);
					return NULL;
				}
				
				//read to rgb arrays
				for(int y = 0; y < h; ++y) {
					for (int x = 0; x < w; ++x) {
						int z = x * 4;
						output[i++] = row_pointers[y][z++]; // red
						output[i++] = row_pointers[y][z++]; // green
						output[i++] = row_pointers[y][z++]; // blue
						output[i++] = row_pointers[y][z++]; // alpha
					}
				}
			}
			else
			{
				png_pixel_bytes = 3;
				size = w * h * 3;
				int i = 0;
				output = malloc(size);
				//malloc failed.
				if (output == NULL)
				{
					fclose(pFile);
					return NULL;
				}
				//read to rgb arrays
				for(int y = 0; y < h; ++y) {
					for (int x = 0; x < w; ++x) {
						int z = x * 4;
						output[i++] = row_pointers[y][z++]; // red
						output[i++] = row_pointers[y][z++]; // green
						output[i++] = row_pointers[y][z++]; // blue
						//skip alpha
					}
				}
			}
		}
			break;
			
		case PNG_COLOR_TYPE_RGB:
		{
			png_pixel_bytes = 3;
			size = w * h * 3;
			output = malloc(size);
			//malloc failed.
			if (output == NULL)
			{
				fclose(pFile);
				return NULL;
			}
			//read to rgb arrays
			int i = 0;
			for(int y = 0; y < h; ++y) {
				for (int x = 0; x < w; ++x) {
					int z = x * 3;
					output[i++] = row_pointers[y][z++]; // red
					output[i++] = row_pointers[y][z++]; // green
					output[i++] = row_pointers[y][z++]; // blue
				}
			}
		}
			break;
		default:
			png_destroy_read_struct( &png_ptr, &info_ptr, 0);
	}
	
	png_destroy_read_struct( &png_ptr, &info_ptr, 0);
	
	//read finish!
	if (width)
		*width = w;
	if (height)
		*height = h;
	if (pixel_bytes)
		*pixel_bytes = png_pixel_bytes;
	
	fclose(pFile);
	
	return output;
}

/**
 *  write rgb to png file
 *
 *  @param png_file_name file name of output png data
 *  @param rgb_data      input rgb data
 *  @param width         width of rgb data
 *  @param height        height of rgb data
 *  @param pixel_bytes   bytes per pixel 3:RBG 4:RGBA
 *
 *  @return 0 sucess or -1 if error.
 */
int aspire_mao_png_write_file(const char* png_file_name, unsigned char* rgb_data, int width, int height, int pixel_bytes)
{
	if (!rgb_data
		|| !width
		|| !height
		|| !(pixel_bytes == 3 || pixel_bytes == 4)) //RGB or RGBA
		return -1;
	
	FILE* pFile = fopen(png_file_name, "wb");
	if (pFile == NULL)
		return -1;
	
	
	//create png struct
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		fclose(pFile);
		return -1;
	}
	//create png info
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		png_destroy_write_struct(&png_ptr,  NULL);
		fclose(pFile);
		return -1;
	}
	
	//set jmp
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(pFile);
		return -1;
	}
	
	//set io
	png_init_io(png_ptr, pFile);
	
	//set ihdr
	if (pixel_bytes == 3)//RGB
		png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB,
					 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	else //RGBA
		png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGBA,
					 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	
	//write png info
	png_write_info(png_ptr, info_ptr);
	
	//alloc mem
	unsigned char **row_pointers = malloc(height * sizeof(unsigned char *));
	for (int i = 0; i < height; ++i) {
		row_pointers[i] = malloc(width * pixel_bytes);
	}
	
	//assign to row_pointers
	int i = 0;
	if (pixel_bytes == 3)
	{
		for (int k = 0; k < height; k++)
			for (int x = 0; x < width; ++x) {
				int z = x * 3;
				row_pointers[k][z++] = rgb_data[i++];
				row_pointers[k][z++] = rgb_data[i++];
				row_pointers[k][z++] = rgb_data[i++];
			}
	}
	else
	{
		for (int k = 0; k < height; k++)
			for (int x = 0; x < width; ++x) {
				int z = x * 4;
				row_pointers[k][z++] = rgb_data[i++];
				row_pointers[k][z++] = rgb_data[i++];
				row_pointers[k][z++] = rgb_data[i++];
				row_pointers[k][z++] = rgb_data[i++];
			}
	}
	
	//write image
	png_write_image(png_ptr, row_pointers);
	
	//write end
	png_write_end(png_ptr, info_ptr);
	
	//release ptr & info
	png_destroy_write_struct(&png_ptr, &info_ptr);
	
	//relase row_pointers
	for (int i = 0; i < height; ++i) {
		free(row_pointers[i]);
	}
	
	free(row_pointers);
	
	fclose(pFile);
	
	return 0;
}


/**
 *  check is png
 *
 *  @param file_name image file name
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_png_file(const char* file_name)
{
	FILE* pFile = fopen(file_name, "rb");
	
	if (pFile == NULL)
		return 0;
	
	//read 4 bytes
	unsigned char buf[4];
	memset(buf, 0, 4);
	size_t readnum = fread(buf, 4, 1, pFile);
	if (readnum != 1)
	{
		fclose(pFile);
		return 0;
	}
	
	//PNG sign check
	if (png_sig_cmp( (png_bytep)buf, (png_size_t)0, 4) == 0)
	{
		fclose(pFile);
		return 1;
	}
	
	fclose(pFile);
	
	return 0;
}


/**
 *  check is png
 *
 *  @param img_data img data
 *  @param img_data_size img data size
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_png(unsigned char* img_data, unsigned int img_data_size)
{
	if (img_data == NULL
		|| img_data_size <= 4)
		return 0;
	
	unsigned char buf[4];
	//PNG sign check
	memset(buf, 0, 4);
	memcpy(buf, img_data, 4);
	
	//img is png
	if (png_sig_cmp( (png_bytep)buf, (png_size_t)0, 4) == 0)
	{
		return 1;
	}
	
	return 0;
}
