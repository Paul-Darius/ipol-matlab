/*
* Copyright (c) 2009-2011, A. Buades <toni.buades@uib.es>, 
* All rights reserved.
*  
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/




#ifndef _LIBAUXILIARY_H_
#define _LIBAUXILIARY_H_



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <time.h>
#include <unistd.h>



#define LUTMAX 30.0
#define LUTMAXM1 29.0
#define LUTPRECISION 1000.0


#define MAX(i,j) ( (i)<(j) ? (j):(i) )
#define MIN(i,j) ( (i)<(j) ? (i):(j) )


#define fTiny 0.00000001f


#define COEFF_YR 0.299
#define COEFF_YG 0.587
#define COEFF_YB 0.114



/**
 * @file   libAuxiliary.cpp
 * @brief  Standard functions used by demosaicking algorithms
 *
 * 
 *
 * @author Antoni Buades <toni.buades@uib.es>
 */




/**
 * \brief  RGV to YUV standard conversion
 *
 * Y = COEFF_YR * R + COEFF_YG  * G + COEFF_YB * B
 *
 * U = R - Y
 *
 * V = B - Y
 *
 * @param[in]  r, g, b  input image
 * @param[out] y, u, v  yuv coordinates 
 * @param[in]  width, height size of the image
 *
 */

void wxRgb2Yuv(float *r,float *g,float *b,float *y,float *u,float *v,int width,int height);



/**
 * \brief   YUV to RGB standard conversion
 *
 *
 * @param[in] y, u, v  yuv coordinates 
 * @param[out]  r, g, b  ouput image
 * @param[in]  width, height size of the image
 *
 */


void wxYuv2Rgb(float *r,float *g,float *b,float *y,float *u,float *v, int width,int height); 



/**
 * \brief  Make a copy of float vector
 *
 *
 * @param[in]  tpI  input vector 
 * @param[out] tpO  copy 
 * @param[in]  ilength length of the vector
 *
 */
void wxCopy(float *tpI,float *tpO, int ilength);



/**
 * \brief  Compute Euclidean distance of a 3x3 patch centered at (i0,j0), (u1,j1) of the same image
 *
 *
 * @param[in]  u0  image
 * @param[in]  (i0,j0)  center of first window
 * @param[in]  (i1,j1)  center of second window
 * @param[in]  width   width of the image
 *
 */

float l2_distance_r1(float *u0, int i0, int j0, int i1,
					 int j1, int width);




/**
 * \brief Tabulate Exp(-x)
 *
 *
 * @param[out]  lut output table
 * @param[in]   size    size of the table
 *
 */

void  sFillLut(float *lut,int size);



/**
 * \brief Compute Exp(-x)
 *
 *
 * @param[in]  dif    value of x
 * @param[in]  lut    table of Exp(-x)
 *
 */

float sLUT(float dif,float *lut); 
									 




/**
 * \brief  Sliding window iterated median filter
 *
 *
 * @param[in]   u  input image
 * @param[out]  v  output image
 * @param[in]  inIter  number of iterations
 * @param[in]  fRadius window of size (2*fRadius+1) x (2*fRadius+1)
 * @param[in]  iWidth, iHeight size of the image
 *
 */

void wxMedian(float *u,float *v, float fRadius, int inIter, int iWidth,int iHeight);



/**
 * \brief  Standard Quicksort. Orders float array in increasing order
 *
 *
 * @param[in]   arr     input array
 * @param[in]   ilength  length of the array
 * @param[out]  arr      output array
 *
 */

void QuickSortFloat(float *arr, int ilength);




#endif
