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


#include "libAuxiliary.h"

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

void wxRgb2Yuv(float *r,float *g,float *b,float *y,float *u,float *v,int width,int height) 
{
	int size=height*width;
	
	for(int i=0;i<size;i++){
		y[i] = ( COEFF_YR *  r[i] + COEFF_YG * g[i] + COEFF_YB * b[i]);
		u[i] =  ( r[i] - y[i]);
		v[i] =  ( b[i] - y[i]);
	}
	
}



/**
 * \brief   YUV to RGB standard conversion
 *
 *
 * @param[in] y, u, v  yuv coordinates 
 * @param[out]  r, g, b  ouput image
 * @param[in]  width, height size of the image
 *
 */


void wxYuv2Rgb(float *r,float *g,float *b,float *y,float *u,float *v, int width,int height)  
{
	
	
	int iwh=height*width;
	
	for(int i=0;i<iwh;i++){
		
		g[i] =  ( y[i] - COEFF_YR * (u[i] + y[i]) - COEFF_YB * (v[i] +  y[i]) ) / COEFF_YG;
		r[i] =  ( u[i] + y[i]);
		b[i] =  ( v[i] + y[i]);
		
	}
	
}



/**
 * \brief  Make a copy of float vector
 *
 *
 * @param[in]  tpI  input vector 
 * @param[out] tpO  copy 
 * @param[in]  ilength length of the vector
 *
 */
void wxCopy(float *tpI,float *tpO, int ilength)
{
	memcpy((void *) tpO, (const void *) tpI, ilength * sizeof(float));
}






/**
 * \brief Tabulate Exp(-x)
 *
 *
 * @param[out]  lut  output table
 * @param[in]   size    size of the table
 *
 */


void  sFillLut(float *lut, int size)
{
	for(int i=0; i< size;i++)   lut[i]=   expf( - (float) i / LUTPRECISION);
}






/**
 * \brief Compute Exp(-x)
 *
 *
 * @param[in]  dif    value of x
 * @param[in]  lut    table of Exp(-x)
 *
 */


float sLUT(float dif, float *lut)
{
  	
	if (dif >= (float) LUTMAXM1) return 0.0;
	
	int  x= (int) floor( (double) dif * (float) LUTPRECISION);
	
	float y1=lut[x];
	float y2=lut[x+1];
	
	return y1 + (y2-y1)*(dif*LUTPRECISION -  x); 
}






/**
 * \brief  Compute Euclidean distance of a 3x3 patch centered at (i0,j0), (u1,j1) of the same image
 *
 *
 * @param[in]  u0  image
 * @param[in]  (i0,j0)  center of first window
 * @param[in]  (i1,j1)  center of second window
 * @param[in]  width    width of the image
 *
 */

float l2_distance_r1(float *u0, int i0, int j0, int i1,
							 int j1, int width)
{
	
    float diff, dist = 0.0;
	
    float *ptr0, *ptr1;
	
    ptr0 = u0 + (j0 - 1) * width + i0 - 1;
    ptr1 = u0 + (j1 - 1) * width + i1 - 1;
	
    /* first line */
	
    diff = *ptr0++ - *ptr1++;
    dist += diff * diff;
    diff = *ptr0++ - *ptr1++;
    dist += diff * diff;
    diff = *ptr0 - *ptr1;
    dist += diff * diff;
	
    /* second line */
    ptr0 += width - 2;
    ptr1 += width - 2;
	
    diff = *ptr0++ - *ptr1++;
    dist += diff * diff;
    diff = *ptr0++ - *ptr1++;
    dist += diff * diff;
    diff = *ptr0 - *ptr1;
    dist += diff * diff;
	
    /* third line */
    ptr0 += width - 2;
    ptr1 += width - 2;
	
    diff = *ptr0++ - *ptr1++;
    dist += diff * diff;
    diff = *ptr0++ - *ptr1++;
    dist += diff * diff;
    diff = *ptr0 - *ptr1;
    dist += diff * diff;
	
    return dist;
}







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




void wxMedian(float *u,float *v, float fRadius, int inIter, int iWidth,int iHeight)
{
    
	
	int iRadius = (int)(fRadius+1.0);
	int iNeigSize = (2*iRadius+1)*(2*iRadius+1);
	float fRadiusSqr = fRadius * fRadius;
	
	
	// Vector to store values of each pixel neighborhood
	float * vector = new float[iNeigSize];
	
	// For each iteration
	for(int n = 0;  n < inIter; n++){
		
		// For each pixel
		for(int x=0;x < iWidth;x++)
			for(int y=0;y< iHeight;y++){
				
				// Take spatial neighborhood fo radius fRadius
				int iCount=0;
				for(int i=-iRadius;i<=iRadius;i++)
					for(int j=-iRadius;j<=iRadius;j++)
						if ((float) (i*i + j*j) <= fRadiusSqr){
							
							int x0=x+i;
							int y0=y+j;
							
							if (x0 >= 0 && y0 >= 0 && x0 < iWidth && y0 < iHeight) { 
								
								vector[iCount] = u[y0*iWidth+x0];
								iCount++; 
								
							}
							
						}
				
				// order neighborhood values
				QuickSortFloat(vector, iCount);
				
				v[y*iWidth+x] = vector[iCount / 2];
				
			}
		
		wxCopy(v,u,iWidth*iHeight);
		
	}
	
	delete[] vector;
	
}








//increasing order

int order_float_increasing(const void *a, const void *b)
{
	if ( *(float*)a  > *(float*)b ) return 1;
	else if ( *(float*)a  < *(float*)b ) return -1;
	
	return 0;
}




/**
 * \brief  Standard Quicksort. Orders float array in increasing order
 *
 *
 * @param[in]   arr      input array
 * @param[in]   ilength  length of the array
 * @param[out]  arr      output array
 *
 */


void QuickSortFloat(float *arr, int ilength)
{
		qsort(arr, ilength, sizeof(float), order_float_increasing);
	
}







