/***********************************************************
 //  resize.c
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

#include "resize.h"
#include <jpeglib.h>
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
	jpeg_mem_dest(&cinfo, &out_buffer, (unsigned long*)jpg_data_size);
	
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

	
	return out_buffer;
}

/**
 *  resize rgb data
 *
 *  @param w_Dest dest width
 *  @param h_Dest dest height
 *  @param src    input rgb data
 *  @param w_Src  src width
 *  @param h_Src  src height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_resize_rgb(int w_Dest, int h_Dest, unsigned char *src, int w_Src, int h_Src, int pixel_bytes)
{
	int sw = w_Src-1, sh = h_Src-1, dw = w_Dest-1, dh = h_Dest-1;
	int B, N, x, y;
	int nPixelSize = pixel_bytes;
	unsigned char *pLinePrev,*pLineNext;
	unsigned char *pDest = malloc(w_Dest*h_Dest*nPixelSize);
	unsigned char *tmp;
	unsigned char *pA,*pB,*pC,*pD;
	
	for(int i=0;i<=dh;++i)
	{
		tmp =pDest + i*w_Dest*nPixelSize;
		y = i*sh/dh;
		N = dh - i*sh%dh;
		pLinePrev = src + (y++)*w_Src*nPixelSize;
		pLineNext = (N==dh) ? pLinePrev : src+y*w_Src*nPixelSize;
		for(int j=0;j<=dw;++j)
		{
			x = j*sw/dw*nPixelSize;
			B = dw-j*sw%dw;
			pA = pLinePrev+x;
			pB = pA+nPixelSize;
			pC = pLineNext + x;
			pD = pC + nPixelSize;
			if(B == dw)
			{
				pB=pA;
				pD=pC;
			}
			
			for(int k=0;k<nPixelSize;++k)
			{
				*tmp++ = ( unsigned char )( int )(
												  ( B * N * ( *pA++ - *pB - *pC + *pD ) + dw * N * *pB++
												   + dh * B * *pC++ + ( dw * dh - dh * B - dw * N ) * *pD++
												   + dw * dh / 2 ) / ( dw * dh ) );
			}
		}
	}
	return pDest;
}


/**
 *  resize png data
 *
 *  @param des_png_data_size size of ouptput png data
 *  @param src_png_data      input png data
 *  @param src_png_data_size size of input png data
 *  @param size          input resized size max height-width
 *
 *  @return output png data or NULL if error.
 */
unsigned char* aspire_mao_resize_png(unsigned int *des_png_data_size, unsigned char* src_png_data, unsigned int src_png_data_size, int size)
{
	if (size == 0)
		return NULL;
	
	int src_width = 0;
	int src_height = 0;
	int pixel_bytes = 3;

	
	//read to rgb buffer
	unsigned char* rgb = aspire_mao_png_read(src_png_data, src_png_data_size, &src_width, &src_height, &pixel_bytes, 0);
	
	if (!rgb || !src_width || !src_height)
		return NULL;
	
	//calc aspect ratio
	float ratio_w = size / (float)src_width;
	float ratio_h = size / (float)src_height;
	float ratio = ratio_w < ratio_h ? ratio_w : ratio_h;
	
	//small image return
	if (ratio >= 1) {
		//free rgb
		free(rgb);
		
		unsigned char* des_png_data = (unsigned char *)malloc(src_png_data_size);
		if (!des_png_data)
			return NULL;
		
		memcpy(des_png_data, (const void *)src_png_data, (size_t)src_png_data_size);
		if (des_png_data_size)
			*des_png_data_size = src_png_data_size;
		
		return des_png_data;
	}
	
	
	//dest size
	int des_width = ratio * src_width;
	int des_height = ratio * src_height;
	
	//resized rgb buffer
	unsigned char * des_rgb = aspire_mao_resize_rgb(des_width, des_height, rgb, src_width, src_height, pixel_bytes);
	
	//free rgb
	free(rgb);

	if (!des_rgb)
		return NULL;
	
	//write to out buffer
	return  aspire_mao_png_write(des_png_data_size, des_rgb, des_width, des_height, pixel_bytes);
}

/**
 *  resize jpeg data
 *
 *  @param des_jpg_data_size  the size of output jpg data
 *  @param src_jpg_data input input jpg data
 *  @param src_jpg_data_size  input the size of input jpg data
 *  @param size    max width-height
 *  @param quality  quality (1-100) max 100 advice use 75
 *
 *  @return output jpg data or NULL if error.
 */
unsigned char* aspire_mao_resize_jpg(unsigned int *des_jpg_data_size, unsigned char* src_jpg_data, unsigned int src_jpg_data_size, int size, int quality)
{
	if (size == 0)
	{
		return NULL;
	}
	
	int src_width = 0;
	int src_height = 0;

	//read to image buffer
	unsigned char* rgb = aspire_mao_jpg_read(src_jpg_data, src_jpg_data_size, &src_width, &src_height);
	
	if (!rgb || !src_width || !src_height)
		return NULL;
	
	//calc aspect ratio
	float ratio_w = size / (float)src_width;
	float ratio_h = size / (float)src_height;
	float ratio = ratio_w < ratio_h ? ratio_w : ratio_h;
	
	//small image return
	if (ratio >= 1) {
		//free rgb
		free(rgb);
		
		unsigned char* des_jpg_data = (unsigned char *)malloc(src_jpg_data_size);
		if (!des_jpg_data)
			return NULL;
		
		memcpy(des_jpg_data, (const void *)src_jpg_data, (size_t)src_jpg_data_size);
		if (des_jpg_data_size)
			*des_jpg_data_size = src_jpg_data_size;
		
		return des_jpg_data;
	}
	
	int width = ratio * src_width;
	int height = ratio * src_height;
	
	//gen resized image buffer
	unsigned char * resized_rgb = aspire_mao_resize_rgb(width, height, rgb, src_width, src_height, 3);
	
	//free rgb
	free(rgb);
	
	if (!resized_rgb)
		return NULL;
	
	//write to out buffer
	unsigned char* output = aspire_mao_jpg_write(des_jpg_data_size, resized_rgb, width, height, quality);
	
	//free rgb
	free(resized_rgb);
	
	return output;
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
		|| img_data_size <= 4)
		return 0;
	
	unsigned char buf[4];
	//jpeg SOI FFD8 && EOI FFD9
	memset(buf, 0, 4);
	memcpy(buf, img_data, 2);//FFD8
	memcpy(buf + 2, img_data + img_data_size - 2, 2); //FFD9
	unsigned char jpgsign[4] = {0xFF, 0xD8, 0xFF, 0xD9};
	if (memcmp(buf, jpgsign, 4) == 0)
	{
		return 1;
	}
	
	return 0;
}

/**
 *  resize image data (only supporrt PNG & JPG)
 *
 *  @param des_img_data_size size of ouptput img data
 *  @param src_img_data      input img data
 *  @param src_img_data_size size of input img data
 *  @param size          input resized size max height-width
 *  @param quality  quality (1-100) max 100 advice use 75 (only for JPG)
 *
 *  @return output img data or NULL if error.
 */
unsigned char* aspire_mao_resize_image(unsigned int *des_img_data_size, unsigned char* src_img_data, unsigned int src_img_data_size, int size, int quality)
{
	//is jpg
	if (aspire_mao_image_is_jpg(src_img_data, src_img_data_size))
	{
		return aspire_mao_resize_jpg(des_img_data_size, src_img_data, src_img_data_size, size, quality);
	}
	
	//is png
	if (aspire_mao_image_is_png(src_img_data, src_img_data_size))
	{
		return aspire_mao_resize_png(des_img_data_size, src_img_data, src_img_data_size, size);
	}
	
	return NULL;
}


/**
 *  resize & convert png to jpg
 *
 *  @param des_jpg_ata_size size of ouptput jpg data
 *  @param src_png_data      input png data
 *  @param src_png_data_size size of input png data
 *  @param size          input resized size max height-width
 *  @param quality       input quality for jpg (1~100) 75 as well
 *
 *  @return output jpg data or NULL if error.
 */
unsigned char* aspire_mao_resize_png_to_jpg(unsigned int *des_jpg_data_size, unsigned char* src_png_data, unsigned int src_png_data_size, int size, int quality)
{
	//check is png or not
	if (!aspire_mao_image_is_png(src_png_data, src_png_data_size))
	{
		return NULL;
	}
	
	if (size == 0)
		return NULL;
	
	int src_width = 0;
	int src_height = 0;
	int pixel_bytes = 3;
	
	
	//read to rgb buffer (need skip alpha)
	unsigned char* rgb = aspire_mao_png_read(src_png_data, src_png_data_size, &src_width, &src_height, &pixel_bytes, 1);
	
	if (!rgb || !src_width || !src_height)
		return NULL;
	
	//calc aspect ratio
	float ratio_w = size / (float)src_width;
	float ratio_h = size / (float)src_height;
	float ratio = ratio_w < ratio_h ? ratio_w : ratio_h;
	
	//dest size
	int des_width = ratio * src_width;
	int des_height = ratio * src_height;
	
	//resized rgb buffer
	unsigned char * des_rgb = aspire_mao_resize_rgb(des_width, des_height, rgb, src_width, src_height, pixel_bytes);
	
	//free rgb
	free(rgb);
	
	if (!des_rgb)
		return NULL;
	
	//write to out buffer
	unsigned char* output = aspire_mao_jpg_write(des_jpg_data_size, des_rgb, des_width, des_height, quality);
	
	//free des_rgb
	free(des_rgb);
	
	return output;
}

/**
 *  resize & convert jpg to png
 *
 *  @param des_png_data_size  size of ouptput jpg data
 *  @param src_jpg_data      input png data
 *  @param src_jpg_data_size size of input png data
 *  @param size              input resized size max height-width
 *
 *  @return output png data or NULL if error.
 */
unsigned char* aspire_mao_resize_jpg_to_png(unsigned int *des_png_data_size, unsigned char* src_jpg_data, unsigned int src_jpg_data_size, int size)
{
	//check is jpg or not
	if (!aspire_mao_image_is_jpg(src_jpg_data, src_jpg_data_size))
	{
		return NULL;
	}
	
	if (size == 0)
		return NULL;
	
	int src_width = 0;
	int src_height = 0;
	int pixel_bytes = 3;
	
	//read to rgb buffer
	unsigned char* rgb = aspire_mao_jpg_read(src_jpg_data, src_jpg_data_size, &src_width, &src_height);
	
	if (!rgb || !src_width || !src_height)
		return NULL;
	
	//calc aspect ratio
	float ratio_w = size / (float)src_width;
	float ratio_h = size / (float)src_height;
	float ratio = ratio_w < ratio_h ? ratio_w : ratio_h;
	
	//dest size
	int des_width = ratio * src_width;
	int des_height = ratio * src_height;
	
	//resized rgb buffer
	unsigned char * des_rgb = aspire_mao_resize_rgb(des_width, des_height, rgb, src_width, src_height, pixel_bytes);
	
	//free rgb
	free(rgb);
	
	if (!des_rgb)
		return NULL;
	
	//write to out buffer
	unsigned char* output = aspire_mao_png_write(des_png_data_size, des_rgb, des_width, des_height, pixel_bytes);
	
	//free des_rgb
	free(des_rgb);
	
	return output;
}

/**
 *  convert png to jpg
 *
 *  @param des_jpg_ata_size size of ouptput jpg data
 *  @param src_png_data      input png data
 *  @param src_png_data_size size of input png data
 *  @param quality       input quality for jpg (1~100) 75 as well
 *
 *  @return output jpg data or NULL if error.
 */
unsigned char* aspire_mao_png_to_jpg(unsigned int *des_jpg_data_size, unsigned char* src_png_data, unsigned int src_png_data_size, int quality)
{
	//check is png or not
	if (!aspire_mao_image_is_png(src_png_data, src_png_data_size))
	{
		return NULL;
	}
	
	int src_width = 0;
	int src_height = 0;
	int pixel_bytes = 3;
	
	//read to rgb buffer (need skip alpha)
	unsigned char* rgb = aspire_mao_png_read(src_png_data, src_png_data_size, &src_width, &src_height, &pixel_bytes, 1);
	
	if (!rgb || !src_width || !src_height)
		return NULL;
	
	//write to out buffer
	unsigned char* output = aspire_mao_jpg_write(des_jpg_data_size, rgb, src_width, src_height, quality);
	
	//free rgb
	free(rgb);
	
	
	return output;
}

/**
 *  convert jpg to png
 *
 *  @param des_png_data_size  size of ouptput jpg data
 *  @param src_jpg_data      input png data
 *  @param src_jpg_data_size size of input png data
 *
 *  @return output png data or NULL if error.
 */
unsigned char* aspire_mao_jpg_to_png(unsigned int *des_png_data_size, unsigned char* src_jpg_data, unsigned int src_jpg_data_size)
{
	//check is jpg or not
	if (!aspire_mao_image_is_jpg(src_jpg_data, src_jpg_data_size))
	{
		return NULL;
	}
	
	int src_width = 0;
	int src_height = 0;
	int pixel_bytes = 3;
	
	//read to rgb buffer
	unsigned char* rgb = aspire_mao_jpg_read(src_jpg_data, src_jpg_data_size, &src_width, &src_height);
	
	if (!rgb || !src_width || !src_height)
		return NULL;
	
	//write to out buffer
	unsigned char* output = aspire_mao_png_write(des_png_data_size, rgb, src_width, src_height, pixel_bytes);
	
	//free rgb
	free(rgb);
	
	return output;
}

