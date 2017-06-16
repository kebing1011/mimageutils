/***********************************************************
 //  rwjpg.c
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

#include "rwjpg.h"
#include <jpeglib.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

/**
 *  jpg error
 */
struct my_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */
	
	jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	
	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);
	
	/* Return control to the setjmp point */
 longjmp(myerr->setjmp_buffer, 1);
}


/**
 *  read jpg to rgb data
 *
 *  @param jpg_data      input jpg data
 *  @param jpg_data_size size of input jpg data
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *
 *  @return rgb data or NULL if error.
 */
unsigned char* aspire_mao_jpg_read(const unsigned char* jpg_data, unsigned int jpg_data_size, int* width, int* height)
{
	struct jpeg_decompress_struct cinfo; //for our jpeg info
	
	// 	struct jpeg_error_mgr err; //the error handler
	// 	info.err = jpeg_std_error(&err);
	struct my_error_mgr my_err;
	
	cinfo.err = jpeg_std_error(&my_err.pub);
	my_err.pub.error_exit = my_error_exit;
	
	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(my_err.setjmp_buffer)) {
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}
	
	jpeg_create_decompress( &cinfo ); //fills info structure
	jpeg_mem_src(&cinfo, (unsigned char *)jpg_data, jpg_data_size);
	int ret_Read_Head = jpeg_read_header( &cinfo, 1 ); //int
	
	if(ret_Read_Head != JPEG_HEADER_OK){
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}
	
	
	boolean bStart = jpeg_start_decompress( &cinfo );
	if(!bStart){
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}
	int w = cinfo.output_width;
	int h = cinfo.output_height;
	int row_stride = cinfo.num_components * w;
	unsigned long dataSize = row_stride * h;
	
	// read RGB(A) scanlines one at a time into jdata[]
	unsigned char *data = (unsigned char *)malloc(dataSize);
	if(!data)
		return NULL;
	
	unsigned char* rowptr;
	while ( cinfo.output_scanline < h )
	{
		rowptr = data + cinfo.output_scanline * row_stride;
		jpeg_read_scanlines( &cinfo, &rowptr, 1 );
	}
	
	//finish
	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress(&cinfo);
	
	//get width & hegith
	if (width)
		*width = w;
	if (height)
		*height = h;
	
	return data;
}

/**
 *  write rgb data to jpg data
 *
 *  @param jpg_data_size output size of jpg data
 *  @param rgb_data      input rgb buffer
 *  @param quality       image quality 1~100 advice 75
 *  @param width         input rgb width
 *  @param height        input rgb height
 *
 *  @return output jpg buffer or NULL if error
 */
unsigned char* aspire_mao_jpg_write (unsigned int* jpg_data_size, const unsigned char* rgb_data, int width, int height, int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);
	
	unsigned char* out_buffer = NULL;
	unsigned long out_size = 0;
	jpeg_mem_dest(&cinfo, &out_buffer, &out_size);
	
	cinfo.image_width = width; 	/* image width and height, in pixels */
	cinfo.image_height = height;
	cinfo.input_components = 3; /* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
	
	jpeg_start_compress(&cinfo, TRUE);
	
	row_stride = width * 3;	/* JSAMPLEs per row in image_buffer */
	
	unsigned char* rgb_ptr = (unsigned char* )rgb_data;
	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = &rgb_ptr[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	
	/*get size out*/
	*jpg_data_size = (unsigned int)out_size;
	
	return out_buffer;
}

/**
 *  read jpg file to rgb data
 *
 *  @param jpg_file_name input jpg file name
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *
 *  @return rgb data or NULL if error.
 */
unsigned char* aspire_mao_jpg_read_file(const char* jpg_file_name, int* width, int* height)
{
	FILE* pFile = fopen(jpg_file_name, "rb");
	if (pFile == NULL)
		return NULL;
	
	struct jpeg_decompress_struct cinfo; //for our jpeg info
	
	// 	struct jpeg_error_mgr err; //the error handler
	// 	info.err = jpeg_std_error(&err);
	struct my_error_mgr my_err;
	
	cinfo.err = jpeg_std_error(&my_err.pub);
	my_err.pub.error_exit = my_error_exit;
	
	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(my_err.setjmp_buffer)) {
		/* If we get here, the JPEG code has signaled an error.
		 * We need to clean up the JPEG object, close the input file, and return.
		 */
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	}
	
	jpeg_create_decompress( &cinfo ); //fills info structure
	jpeg_stdio_src(&cinfo, pFile);
	int ret_Read_Head = jpeg_read_header( &cinfo, 1 ); //int
	
	if(ret_Read_Head != JPEG_HEADER_OK){
		jpeg_destroy_decompress(&cinfo);
		fclose(pFile);
		
		return NULL;
	}
	
	
	boolean bStart = jpeg_start_decompress( &cinfo );
	if(!bStart){
		jpeg_destroy_decompress(&cinfo);
		fclose(pFile);
		
		return NULL;
	}
	int w = cinfo.output_width;
	int h = cinfo.output_height;
	int row_stride = cinfo.num_components * w;
	unsigned long dataSize = row_stride * h;
	
	// read RGB(A) scanlines one at a time into jdata[]
	unsigned char *data = (unsigned char *)malloc(dataSize);
	if(data == NULL) {
		fclose(pFile);
		return NULL;
	}
	unsigned char* rowptr;
	while ( cinfo.output_scanline < h )
	{
		rowptr = data + cinfo.output_scanline * row_stride;
		jpeg_read_scanlines( &cinfo, &rowptr, 1 );
	}
	
	//finish
	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress(&cinfo);
	
	//get width & hegith
	if (width)
		*width = w;
	if (height)
		*height = h;
	
	fclose(pFile);
	
	return data;
}


/**
 *  write rgb data to jpg file
 *
 *  @param jpg_file_name output jpg file name
 *  @param rgb_data      input rgb buffer
 *  @param quality       image quality 1~100 advice 75
 *  @param width         input rgb width
 *  @param height        input rgb height
 *
 *  @return 0 sucess or -1 if faild
 */
int aspire_mao_jpg_write_file (const char* jpg_file_name, const unsigned char* rgb_data, int width, int height, int quality)
{
	FILE* pFile = fopen(jpg_file_name, "wb");
	if (pFile == NULL)
		return -1;
	
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);
	
	jpeg_stdio_dest(&cinfo, pFile);
	
	cinfo.image_width = width; 	/* image width and height, in pixels */
	cinfo.image_height = height;
	cinfo.input_components = 3; /* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
	
	jpeg_start_compress(&cinfo, TRUE);
	
	row_stride = width * 3;	/* JSAMPLEs per row in image_buffer */
	
	unsigned char* rgb_ptr = (unsigned char* )rgb_data;
	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = &rgb_ptr[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	
	fclose(pFile);
	
	return 0;
}


/**
 *  check is jpg
 *
 *  @param img_data img data
 *  @param img_data_size img data size
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_jpg(unsigned char* img_data, unsigned int img_data_size)
{
	if (img_data == NULL
		|| img_data_size <= 2)
		return 0;
	
	unsigned char buf[2];
	//jpeg SOI FFD8 && EOI FFD9
	memset(buf, 0, 2);
	memcpy(buf, img_data, 2);//FFD8
	/* some jpeg not suffix with FFD9 */
//	memcpy(buf + 2, img_data + img_data_size - 2, 2); //FFD9
	unsigned char jpgsign[2] = {0xFF, 0xD8};
	if (memcmp(buf, jpgsign, 2) == 0)
	{
		return 1;
	}
	
	return 0;
}


/**
 *  check is jpg
 *
 *  @param file_name image file name
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_jpg_file(const char* file_name)
{
	FILE* pFile = fopen(file_name, "rb");
	
	if (pFile == NULL)
		return 0;
	
	//jpeg SOI FFD8 && EOI FFD9
	unsigned char buf[2];
	memset(buf, 0x0, 2);
	
	size_t readnum = fread(buf, 2, 1, pFile);//FFD8
	if (readnum != 1)
	{
		fclose(pFile);
		return 0;
	}
// some jpeg not suffix with FFD9
//	fseek(pFile, -2, SEEK_END);
//	readnum = fread(buf + 2, 2, 1, pFile);//FFD9
//	if (readnum != 1)
//	{
//		fclose(pFile);
//		return 0;
//	}
	
	unsigned char jpgsign[2] = {0xFF, 0xD8};
	if (memcmp(buf, jpgsign, 2) == 0)
	{
		fclose(pFile);
		return 1;
	}
	
	fclose(pFile);
	
	return 0;
}

