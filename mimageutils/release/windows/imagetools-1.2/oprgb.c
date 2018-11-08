/***********************************************************
 //  oprgb.c
 //  Mao Kebing
 //  Created by Mao on 13-7-25.
 //  Copyright (c) 2013 Eduapp. All rights reserved.
 ***********************************************************/

#include "oprgb.h"
#include <stdlib.h>
#include <string.h>


/**
 *  resize rgb data
 *
 *  @param dest_width  dest width
 *  @param dest_height dest height
 *  @param rgb         input rgb data
 *  @param src_width   src width
 *  @param src_height  src height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_resize_rgb(int dest_width, int dest_height, unsigned char *rgb, int src_width, int src_height, int pixel_bytes)
{
	int sw = src_width-1, sh = src_height-1, dw = dest_width-1, dh = dest_height-1;
	int B, N, x, y;
	unsigned char *pLinePrev,*pLineNext;
	unsigned char *pDest = malloc(dest_width*dest_height*pixel_bytes);
	unsigned char *tmp;
	unsigned char *pA,*pB,*pC,*pD;
	
	for(int i=0;i<=dh;++i)
	{
		tmp =pDest + i*dest_width*pixel_bytes;
		y = i*sh/dh;
		N = dh - i*sh%dh;
		pLinePrev = rgb + (y++)*src_width*pixel_bytes;
		pLineNext = (N==dh) ? pLinePrev : rgb+y*src_width*pixel_bytes;
		for(int j=0;j<=dw;++j)
		{
			x = j*sw/dw*pixel_bytes;
			B = dw-j*sw%dw;
			pA = pLinePrev+x;
			pB = pA+pixel_bytes;
			pC = pLineNext + x;
			pD = pC + pixel_bytes;
			if(B == dw)
			{
				pB=pA;
				pD=pC;
			}
			
			for(int k=0;k<pixel_bytes;++k)
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
 *  flip rgb data
 *
 *  @param rgb     input rgb data
 *  @param width   width
 *  @param width   height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_rgb_flip_horizontal(unsigned char *rgb, int width, int height, int pixel_bytes)
{
	unsigned char* des_rgb = malloc(width * height * pixel_bytes);
	if (des_rgb == NULL)
		return NULL;
	
	//flip
	unsigned char* pDest = des_rgb;
	for (int i = 0; i < height; ++i) {
		for (int j = width - 1; j >= 0; --j) {
			unsigned char* pSrc = rgb + i * width * pixel_bytes + j * pixel_bytes;
			memcpy(pDest, pSrc, pixel_bytes);
			pDest+= pixel_bytes;
		}
	}
	
	return des_rgb;
}


/**
 *  flip rgb data
 *
 *  @param rgb     input rgb data
 *  @param width   width
 *  @param width   height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_rgb_flip_vertical(unsigned char *rgb, int width, int height, int pixel_bytes)
{
	unsigned char* des_rgb = malloc(width * height * pixel_bytes);
	if (des_rgb == NULL)
		return NULL;
	
	//flip
	unsigned char* pDest = des_rgb;
	for (int i = height - 1; i >= 0; --i) {
		for (int j = 0; j < width; ++j) {
			unsigned char* pSrc = rgb + i * width * pixel_bytes + j * pixel_bytes;
			memcpy(pDest, pSrc, pixel_bytes);
			pDest+= pixel_bytes;
		}
	}
	
	return des_rgb;
}


/**
 *  rotate rgb data
 *
 *  @param rgb     input rgb data
 *  @param width   width
 *  @param width   height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_rgb_rotate_right(unsigned char *rgb, int width, int height, int pixel_bytes)
{
	unsigned char* des_rgb = malloc(width * height * pixel_bytes);
	if (des_rgb == NULL)
		return NULL;
	
	//flip
	unsigned char* pDest = des_rgb;
	for (int j = 0; j < width; ++j) {
		for (int i = height - 1; i >= 0; --i) {
			unsigned char* pSrc = rgb + i * width * pixel_bytes + j * pixel_bytes;
			memcpy(pDest, pSrc, pixel_bytes);
			pDest+= pixel_bytes;
		}
	}
	
	return des_rgb;
}

/**
 *  rotate rgb data
 *
 *  @param rgb     input rgb data
 *  @param width   width
 *  @param width   height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_rgb_rotate_left(unsigned char *rgb, int width, int height, int pixel_bytes)
{
	unsigned char* des_rgb = malloc(width * height * pixel_bytes);
	if (des_rgb == NULL)
		return NULL;
	
	//flip
	unsigned char* pDest = des_rgb;
	for (int j = width - 1; j >= 0; --j) {
		for (int i = 0; i < height; ++i) {
			unsigned char* pSrc = rgb + i * width * pixel_bytes + j * pixel_bytes;
			memcpy(pDest, pSrc, pixel_bytes);
			pDest+= pixel_bytes;
		}
	}
	
	return des_rgb;
}


/**
 *  create rgb data
 *
 *  @param rgba_str     input rgba eg. ff00ffff RGBA
 *  @param width   width
 *  @param width   height
 *
 *  @return output rgb buffer or NULL if error.
 */
unsigned char* aspire_mao_rgb_create(char* rgba_str,
									 int width,
									 int height,
									 int skip_alpha)
{
	if (rgba_str == NULL)
		return NULL;
	
	size_t len = strlen(rgba_str);
	if (len != 6 && len != 8)
	{
		printf("rgba use RGBA or RGB eg. ff00ff00 ff00ff\n");
		return NULL;
	}

	unsigned char rgba[4] = {0};
	int pixel_bytes = 0;

	//rgba
	if (len == 8 && !skip_alpha)
	{
		pixel_bytes = 4;
		long long rgball = strtol(rgba_str, NULL, 16);
		rgba[3] = (unsigned char) (rgball & 0xFF);
		rgba[2] = (unsigned char) ((rgball >> 8) & 0xFF);
		rgba[1] = (unsigned char) ((rgball >> 16) & 0xFF);
		rgba[0] = (unsigned char) ((rgball >> 24) & 0xFF);
	}
	else
	{
		pixel_bytes = 3;
		long long rgball = strtol(rgba_str, NULL, 16);
		if (len == 8)
			rgball = (rgball >> 8) & 0xFFFFFF;//skip alpha
		rgba[2] = (unsigned char) (rgball & 0xFF);
		rgba[1] = (unsigned char) ((rgball >> 8) & 0xFF);
		rgba[0] = (unsigned char) ((rgball >> 16) & 0xFF);
	}

	
	unsigned char* output = malloc(width * height * pixel_bytes);
	
	if (output == NULL)
		return NULL;
	
	//assign rgb to output
	unsigned char* pRgb = output;
	
	for (int i = 0; i < height * width; ++i) {
		memcpy(pRgb, rgba, pixel_bytes);
		pRgb += pixel_bytes;
	}
	
	return output;
}
