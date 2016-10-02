//
//  resizergb.c
//  mjpeg
//
//  Created by Mao on 28/09/2016.
//  Copyright © 2016 Maokebing. All rights reserved.
//

#include "resizergb.h"
#include <stdlib.h>



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
unsigned char* aspire_mao_rgb_flip_horizontal(unsigned char *rgb, int width, int height, int pixel_bytes)
{
	unsigned char temp[pixel_bytes];
	memset(temp, 0, pixel_bytes);
	
	return NULL;
}


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
unsigned char* aspire_mao_rgb_flip_vertical(unsigned char *rgb, int width, int height, int pixel_bytes)
{
	return NULL;
}

