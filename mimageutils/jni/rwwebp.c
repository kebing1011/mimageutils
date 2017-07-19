//
//  rwwebp.c
//  mimageutils
//
//  Created by Mao on 19/07/2017.
//  Copyright © 2017 Maokebing. All rights reserved.
//

#include "rwwebp.h"
#include <stdlib.h>
#include <string.h>
#include <webp/decode.h>
#include <webp/encode.h>


/**
 *  check is webp
 *
 *  @param img_data img data
 *  @param img_data_size img data size
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_webp(const unsigned char* img_data, unsigned int img_data_size)
{
	if (img_data == NULL
		|| img_data_size < 12)
		return 0;
	
	unsigned char buf[12];
	memset(buf, 0, 12);
	memcpy(buf, img_data, 12);
	
	/* webp format is RIFF && WEBP in first 12 bytes*/
	unsigned char webpsign_prefix[4] = {'R','I', 'F', 'F'};
	unsigned char webpsign_suffix[4] = {'W','E', 'B', 'P'};

	if (memcmp(buf, webpsign_prefix, 4) == 0
		&& memcmp(buf + 8, webpsign_suffix, 4) == 0)
	{
		return 1;
	}
	
	return 0;
}

/**
 *  check is webp
 *
 *  @param file_name image file name
 *
 *  @return 1:is 0:not
 */
int aspire_mao_image_is_webp_file(const char* file_name)
{
	FILE* fp = fopen(file_name, "rb");
	
	if (fp == NULL)
		return 0;
	
	unsigned char buf[12];
	size_t read = fread(buf, 1, 12, fp);
	fclose(fp);
	
	if (read != 12)
		return 0;
	
	return aspire_mao_image_is_webp(buf, 12);
}

/**
 *  read webp to rgb data
 *
 *  @param webp_data      input jpg data
 *  @param webp_data_size size of input webp data
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *  @param skip_alpha        skip alpha channel
 *  @param pixel_bytes        3 or 4 rgba
 *
 *  @return rgb data or NULL if error.
 */
unsigned char* aspire_mao_webp_read(const unsigned char* webp_data,
									unsigned int webp_data_size,
									int* width,
									int* height,
									int* pixel_bytes,
									int skip_alpha)
{
	/*
	if (webp_data == NULL
		|| width == NULL
		|| height == NULL)
		return NULL;
	
	if (!aspire_mao_image_is_webp(webp_data, webp_data_size))
		return NULL;
	
	WebPDecoderConfig config;
	if (!WebPInitDecoderConfig(&config))
		return NULL;

	if (WebPDecode(webp_data, webp_data_size, &config) != VP8_STATUS_OK)
		return NULL;
	
	WEBP_CSP_MODE colorspace = config.output.colorspace;
	int pixels = 3;
	if (WebPIsAlphaMode(colorspace))
		pixels = 4;
	
	//output
	*height = (int)(config.output.u.RGBA.size / config.output.u.RGBA.stride);
	*width = (int)(config.output.u.RGBA.stride / pixels);
	*pixel_bytes = pixels;
	
	return config.output.u.RGBA.rgba;
	*/
	
	if (webp_data == NULL
		|| width == NULL
		|| height == NULL
		|| pixel_bytes == NULL)
		return NULL;
	
	*pixel_bytes = 4;
	if (skip_alpha)
	{
		*pixel_bytes = 3;
		return WebPDecodeRGB(webp_data, webp_data_size, width, height);
	}
	
	return WebPDecodeBGRA(webp_data, webp_data_size, width, height);
}


/**
 *  read webp file to rgb data
 *
 *  @param webp_data      input jpg data
 *  @param webp_data_size size of input webp data
 *  @param width         output width of rgb
 *  @param height        output height of rgb
 *  @param skip_alpha        skip alpha channel
 *
 *  @return rgb data or NULL if error.
 */
unsigned char* aspire_mao_webp_read_file(const char* webp_file_name,
										 int* width,
										 int* height,
										 int* pixel_bytes,
										 int skip_alpha)
{
	FILE* fp = fopen(webp_file_name, "rb");
	if (fp == NULL)
		return NULL;
	
	fseek(fp, 0, SEEK_END);
	size_t webp_len = (size_t )ftell(fp);
	unsigned char *webp_buf = malloc(webp_len);
	if (webp_buf == NULL)
		return NULL;
	
	rewind(fp);
	fread(webp_buf, webp_len, 1, fp);
	fclose(fp);
	
	unsigned char* output = aspire_mao_webp_read(webp_buf, (int)webp_len, width, height, pixel_bytes, skip_alpha);
	free(webp_buf);
	
	return output;
}


/**
 *  write rgb data to webp data
 *
 *  @param webp_data_size output size of webp data
 *  @param rgb_data      input rgb buffer
 *  @param quality       image quality 1~100 advice 75
 *  @param width         input rgb width
 *  @param height        input rgb height
 *
 *  @return output jpg buffer or NULL if error
 */
unsigned char* aspire_mao_webp_write (unsigned int* webp_data_size,
									 const unsigned char* rgb_data,
									 int width,
									 int height,
									 int pixel_bytes,
									 int quality)
{
	unsigned char* output = NULL;
	
	if (pixel_bytes == 3) {
		*webp_data_size = (unsigned int)WebPEncodeRGB(rgb_data, width, height, width * pixel_bytes, quality, &output);
	}else if (pixel_bytes == 4) {
		*webp_data_size = (unsigned int)WebPEncodeRGBA(rgb_data, width, height, width * pixel_bytes, quality, &output);
	}
	
	return output;
}


/**
 *  write rgb data to webp file
 *
 *  @param webp_file_name filename
 *  @param rgb_data      input rgb buffer
 *  @param quality       image quality 1~100 advice 75
 *  @param width         input rgb width
 *  @param height        input rgb height
 *
 *  @return output jpg buffer or NULL if error
 */
int aspire_mao_webp_write_file (const char* webp_file_name,
								const unsigned char* rgb_data,
								int width,
								int height,
								int pixel_bytes,
								int quality)
{
	FILE* fp = fopen(webp_file_name, "wb");
	if (fp == NULL)
		return -1;
	
	unsigned int webp_data_size = 0;
	unsigned char* webp_buf = aspire_mao_webp_write(&webp_data_size, rgb_data, width, height, pixel_bytes, quality);
	
	if (webp_buf == NULL)
		return -1;

	fwrite(webp_buf, webp_data_size, 1, fp);
	fclose(fp);
	
	free(webp_buf);
	
	return 0;
}

