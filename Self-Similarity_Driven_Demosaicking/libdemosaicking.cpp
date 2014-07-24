/*
* Copyright (c) 2009-2011, A. Buades <toni.buades@uib.es>
* All rights reserved.
*  
*
* Patent warning:
*
* This file implements algorithms possibly linked to the patents
* 
* # J. Hamilton Jr and J. Adams Jr, “Adaptive color plan interpolation
* in single sensor color electronic camera,” 1997, US Patent 5,629,734.
* 
* # D. Cok, “Signal processing method and apparatus for producing
* interpolated chrominance values in a sampled color image signal”,
* 1987, US Patent 4,642,678.
* 
* # A. Buades, T. Coll and J.M. Morel, Image data processing method by
* reducing image noise, and camera integrating means for implementing
* said method, EP Patent 1,749,278 (Feb. 7, 2007). 
* 
* This file is made available for the exclusive aim of serving as
* scientific tool to verify the soundness and completeness of the
* algorithm description. Compilation, execution and redistribution
* of this file may violate patents rights in certain countries.
* The situation being different for every country and changing
* over time, it is your responsibility to determine which patent
* rights restrictions apply to you before you compile, use,
* modify, or redistribute this file. A patent lawyer is qualified
* to make this determination.
* If and only if they don't conflict with any patent terms, you
* can benefit from the following license terms attached to this
* file.
* 
* License:
*
* This program is provided for scientific and educational only:
* you can use and/or modify it for these purposes, but you are
* not allowed to redistribute this work or derivative works in
* source or executable form. A license must be obtained from the
* patent right holders for any other use.
*
*
*/


#include "libdemosaicking.h"


#define GREENPOSITION 0
#define REDPOSITION 1
#define BLUEPOSITION 2




/**
 * @file   libdemosaicking.cpp
 * @brief  Demosaicking functions: HAmilton-Adams algorithm, NLmeans based demosaicking, Chromatic components filtering
 *
 * 
 *
 * @author Antoni Buades <toni.buades@uib.es>
 */




/**
 * \brief  Classical Adams-Hamilton demosaicking algorithm
 *
 *  The green channel is interpolated directionally depending on the green first and red and blue second directional derivatives.
 *  The red and blue differences with the green channel are interpolated bilinearly.
 *
 * @param[in]  ired, igreen, iblue  original cfa image
 * @param[out] ored, ogreen, oblue  demosaicked output 
 * @param[in]  (redx, redy)  coordinates of the red pixel: (0,0), (0,1), (1,0), (1,1)
 * @param[in]  threshold value to consider horizontal and vertical variations equivalent and average both estimates
 * @param[in]  width, height size of the image
 *
 */



void demosaicking_adams(float threshold, int redx,int redy,float *ired,float *igreen,float *iblue,float *ored,float *ogreen,float *oblue,int width,int height)
{
	
	
	
	wxCopy(ired,ored,width*height);
	wxCopy(igreen,ogreen,width*height);
	wxCopy(iblue,oblue,width*height);
	
	
	// Initializations
	int bluex = 1 - redx;
	int bluey = 1 - redy;
	
	
	// CFA Mask of color per pixel
	unsigned char* mask = (unsigned char *) malloc(width*height*sizeof(unsigned char));
	
	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++){
			
			if (x%2 == redx && y%2 == redy) mask[y*width+x] = REDPOSITION;
			else  if (x%2 == bluex && y%2 == bluey) mask[y*width+x] = BLUEPOSITION;
			else 	mask[y*width+x] = GREENPOSITION;
			
		}
	
	
	
	
	// Interpolate the green channel by bilinear on the boundaries  
	// make the average of four neighbouring green pixels: Nourth, South, East, West
	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++)
			if ( (mask[y*width+ x] != GREENPOSITION) && (x < 3 || y < 3 || x>= width - 3 || y>= height - 3 )) { 
				
				
				int gn, gs, ge, gw;
				
				if (y > 0)  gn = y-1;	else    gn = 1;
				if (y < height-1)	gs = y+1;  else  gs = height - 2 ;
				if (x < width-1)  ge = x+1;  else  ge = width-2;
				if (x > 0) gw = x-1;	else  gw = 1;
				
				ogreen[y*width + x] = (ogreen[gn*width + x] +  ogreen[gs*width + x] + ogreen[y*width + gw] +  ogreen[y*width + ge])/ 4.0;
				
			}
	
	
	
	// Interpolate the green by Adams algorithm inside the image    
	// First interpolate green directionally
	for(int x=3;x<width-3;x++)
		for(int y=3;y<height-3;y++)
			if (mask[y*width+ x] != GREENPOSITION ) {  
				
				
				int l = y*width+x;
				int lp1 = (y+1)*width +x;
				int lp2 = (y+2)*width +x;
				int lm1 = (y-1)*width +x;
				int lm2 = (y-2)*width +x;
				
				// Compute vertical and horizontal gradients in the green channel
				float adv = fabsf(ogreen[lp1] - ogreen[lm1]);
				float adh = fabsf(ogreen[l-1] - ogreen[l+1]);
				float dh0, dv0;
				
				// If current pixel is blue, we compute the horizontal and vertical blue second derivatives
				// else is red, we compute the horizontal and vertical red second derivatives
				if (mask[l] == BLUEPOSITION){  
					
					dh0 = 2.0 * oblue[l] - oblue[l+2] - oblue[l-2];	
					dv0 = 2.0 * oblue[l] - oblue[lp2] - oblue[lm2];
					
				} else { 			
					
					
					dh0 = 2.0 * ored[l] - ored[l+2] - ored[l-2];	
					dv0 = 2.0 * ored[l] - ored[lp2] - ored[lm2];
					
				}	
				
				// Add vertical and horizontal differences
				adh = adh + fabsf(dh0);
				adv = adv + fabsf(dv0);
				
				// If vertical and horizontal differences are similar, compute an isotropic average
				if (fabsf(adv - adh) < threshold)
					
					ogreen[l] = (ogreen[lm1] +  ogreen[lp1] +  ogreen[l-1] + ogreen[l+1]) /4.0 + (dh0 + dv0) / 8.0;
				
				// Else If horizontal differences are smaller, compute horizontal average
				else if (adh < adv )
					
					ogreen[l] = (ogreen[l-1] + ogreen[l+1])/2.0 + (dh0)/4.0;
				
				// Else If vertical differences are smaller, compute vertical average			
				else if ( adv < adh ) 			
					
					ogreen[l] = (ogreen[lp1] + ogreen[lm1])/2.0 + (dv0)/4.0;
				
			}
	
	
	// compute the bilinear on the differences of the red and blue with the already interpolated green
	demosaicking_bilinear_red_blue(redx,redy,ored,ogreen,oblue,width,height);
	
	
	free(mask);
}




/**
 * \brief  Classical bilinear interpolation of red and blue differences with the green channel
 *
 *
 * @param[in]  ored, ogreen, oblue  original cfa image with green interpolated
 * @param[out] ored, ogreen, oblue  demosaicked output 
 * @param[in]  (redx, redy)  coordinates of the red pixel: (0,0), (0,1), (1,0), (1,1)
 * @param[in]  width, height size of the image
 *
 */

void demosaicking_bilinear_red_blue(int redx,int redy,float *ored,float *ogreen,float *oblue,int width,int height)
{
	
	
	
	//Initializations 
	int bluex = 1 - redx;
	int bluey = 1 - redy;
	
	
	// Mask of color per pixel
	unsigned char* mask = (unsigned char *) malloc(width*height*sizeof(unsigned char));
	
	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++){
			
			if (x%2 == redx && y%2 == redy) mask[y*width+x] = REDPOSITION;
			else  if (x%2 == bluex && y%2 == bluey) mask[y*width+x] = BLUEPOSITION;
			else 	mask[y*width+x] = GREENPOSITION;
			
		}
	
	
	// Compute the differences  
	for(int i=0; i < width*height;i++){
		
		ored[i] -= ogreen[i];
		oblue[i] -= ogreen[i];
		
	}
	
	
	
	// Interpolate the blue differences making the average of possible values depending on the CFA structure 
	for(int x=0; x < width;x++)
		for(int y=0; y < height;y++)
			if (mask[y*width+x] != BLUEPOSITION){
				
				int gn, gs, ge, gw;
				
				// Compute north, south, west, east positions
				// taking a mirror symmetry at the boundaries
				if (y > 0)  gn = y-1;	else    gn = 1;
				if (y < height-1)	gs = y+1;  else  gs = height - 2 ;
				if (x < width-1)  ge = x+1;  else  ge = width-2;
				if (x > 0) gw = x-1;	else  gw = 1;
				
				if (mask[y*width+x] == GREENPOSITION && y % 2 == bluey) 
					oblue[y*width+x] = ( oblue[y*width+ge] + oblue[y*width+gw])/2.0;
				else if (mask[y*width+x] == GREENPOSITION  && x % 2 == bluex) 
					oblue[y*width+x] = ( oblue[gn*width+x] + oblue[gs*width+x])/2.0;
				else {
					oblue[y*width+x] =  (oblue[gn*width+ge] + oblue[gn*width + gw]  +  oblue[gs*width + ge] +  oblue[gs*width +gw])/4.0;		 
				}
				
			}
	
	
	
	
	// Interpolate the blue differences making the average of possible values depending on the CFA structure
	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++)
			if (mask[y*width+x] != REDPOSITION){
				
				int gn, gs, ge, gw;
				
				// Compute north, south, west, east positions
				// taking a mirror symmetry at the boundaries
				if (y > 0)  gn = y-1;	else    gn = 1;
				if (y < height-1)	gs = y+1;  else  gs = height - 2 ;
				if (x < width-1)  ge = x+1;  else  ge = width-2;
				if (x > 0) gw = x-1;	else  gw = 1;
				
				if (mask[y*width+x] == GREENPOSITION && y % 2 == redy) 
					ored[y*width+x] = ( ored[y*width+ge] + ored[y*width+gw])/2.0;
				else if (mask[y*width+x] == GREENPOSITION  && x % 2 == redx) 
					ored[y*width+x] = ( ored[gn*width+x] + ored[gs*width+x])/2.0;
				else {
					ored[y*width+x] =  (ored[gn*width+ge] + ored[gn*width + gw]  +  ored[gs*width + ge] +  ored[gs*width +gw])/4.0;	 
				}
				
			}
	
	
	// Make back the differences
	for(int i=0;i<width*height;i++){
		
		ored[i] += ogreen[i];
		oblue[i] += ogreen[i];
	}
	
	
	
	free(mask);
	
}







/**
 * \brief  NLmeans based demosaicking
 *
 * For each value to be filled, a weigthed average of original CFA values of the same channel is performed.
 * The weight depends on the difference of a 3x3 color patch 
 *
 * @param[in]  ired, igreen, iblue  initial demosaicked image
 * @param[out] ored, ogreen, oblue  demosaicked output 
 * @param[in]  (redx, redy)  coordinates of the red pixel: (0,0), (0,1), (1,0), (1,1)
 * @param[in]  bloc  research block of size (2+bloc+1) x (2*bloc+1)
 * @param[in]  h kernel bandwidth 
 * @param[in]  width, height size of the image
 *
 */


void demosaicking_nlmeans(int bloc, float h,int redx,int redy,float *ired,float *igreen,float *iblue,float *ored,float *ogreen,float *oblue,int width,int height)
{
	
	
	// Initializations
	int bluex = 1 - redx;
	int bluey = 1 - redy;

	
	
	// CFA Mask of color per pixel
	unsigned char *cfamask = new unsigned char[width*height];
	
	
	for(int x=0;x<width;x++)
		for(int y=0;y<height;y++){
			
			if (x%2 == redx && y%2 == redy) cfamask[y*width+x] = REDPOSITION;
			else  if (x%2 == bluex && y%2 == bluey) cfamask[y*width+x] = BLUEPOSITION;
			else 	cfamask[y*width+x] = GREENPOSITION;
			
		}
	
	
	wxCopy(ired,ored,width*height);
	wxCopy(igreen,ogreen,width*height);
	wxCopy(iblue,oblue,width*height);
	
	
	
	// Tabulate the function Exp(-x) for x>0.
	int luttaille = (int) (LUTMAX*LUTPRECISION);
	float *lut = new float[luttaille];
	
	sFillLut(lut, luttaille);
	
	
	
	
	// for each pixel
	for(int y=2; y <height-2; y++)
		for(int x=2; x<width-2; x++)
		{
			// index of current pixel
			int l=y*width+x;
			
			
			// Learning zone depending on the window size
			int imin=MAX(x-bloc,1);
			int jmin=MAX(y-bloc,1);
			
			int imax=MIN(x+bloc,width-2);
			int jmax=MIN(y+bloc,height-2);
			
			
			// auxiliary variables for computing average
			float red=0.0;
			float green=0.0;
			float blue=0.0;
			
			float rweight=0.0;
			float gweight=0.0;
			float bweight=0.0;
			
			
			// for each pixel in the neighborhood
			for(int j=jmin;j<=jmax;j++)
				for(int i=imin;i<=imax;i++) {
					
					// index of neighborhood pixel
					int l0=j*width+i;
					
					// We only interpolate channels differents of the current pixel channel
					if (cfamask[l]!=cfamask[l0]) {
						
						
						// Distances computed on color
						float some = 0.0;
						
						some = l2_distance_r1(ired,  x, y, i,
											  j, width);
						some += l2_distance_r1(igreen,  x, y, i,
											   j, width);
						some += l2_distance_r1(iblue,  x, y, i,
											   j, width);
						
						
						
						// Compute weight
						some= some / (27.0 * h);
						
						float weight = sLUT(some,lut);
						
						// Add pixel to corresponding channel average
						
						if (cfamask[l0] == GREENPOSITION)  {
							
							green += weight*igreen[l0];
							gweight+= weight;
							
						} else if (cfamask[l0] == REDPOSITION) {
							
							red += weight*ired[l0];
							rweight+= weight;
							
						} else {
							
							blue += weight*iblue[l0];
							bweight+= weight;
						}
						
					}
					
				}
			
			
			// Set value to current pixel
			if (cfamask[l] != GREENPOSITION && gweight > fTiny)  ogreen[l]  =   green / gweight; 
			else  ogreen[l] = igreen[l];
			
			if ( cfamask[l] != REDPOSITION && rweight > fTiny)  ored[l]  =  red / rweight ;
			else    ored[l] = ired[l];
			
			if  (cfamask[l] != BLUEPOSITION && bweight > fTiny)   oblue[l] =  blue / bweight;
			else  oblue[l] = iblue[l];
			
			
		} 
	
	delete[] cfamask;
	delete[] lut;
	
}




/**
 * \brief  Iterate median filter on chromatic components of the image
 *
 *
 * @param[in]  ired, igreen, iblue  initial  image
 * @param[in]  iter  number of iteracions
 * @param[out] ored, ogreen, oblue  filtered output 
 * @param[in]  (redx, redy)  coordinates of the red pixel: (0,0), (0,1), (1,0), (1,1)
 * @param[in]  side  median in a (2*side+1) x (2*side+1) window
 * @param[in]  projflag if not zero, values of the original CFA are kept 
 * @param[in]  width, height size of the image
 *
 */


void chromatic_median(int iter,int redx,int redy,int projflag,float side,float *ired,float *igreen, float *iblue,float *ored,float *ogreen,float *oblue,int width,int height)
{
	
	
	int size=height*width;
	
	// Auxiliary variables for computing chromatic components
	float *y=new float[size];
	float *u=new float[size];
	float *v=new float[size];
	float *u0=new float[size];
	float *v0=new float[size];
	
	int bluex=1-redx;
	int bluey=1-redy;
	
	
	// For each iteration
	for(int i=1;i<=iter;i++){
		
		
		// Transform to YUV
		wxRgb2Yuv(ired,igreen,iblue,y,u,v,width,height); 
		
		// Perform a Median on YUV component
		wxMedian(u,u0,side,1 ,width,height); 
		wxMedian(v,v0,side,1 ,width,height); 
		
		
		// Transform back to RGB
		wxYuv2Rgb(ored,ogreen,oblue,y,u0,v0,width,height); 
		
		// If projection flag activated put back original CFA values
		if (projflag)
		{
		
			for(int x=0;x<width;x++)
				for(int y=0;y<height;y++){
					
					int l=y*width+x;
					
					if (x%2==redx && y%2==redy) ored[l]=ired[l];
					
					else if (x%2==bluex && y%2==bluey) oblue[l]=iblue[l];
					
					else ogreen[l]=igreen[l];
					
				}
			
		}
		
		
		wxCopy(ored,ired,size);
		wxCopy(ogreen,igreen,size);
		wxCopy(oblue,iblue,size);
		
	}

	
	// delete auxiliary memory
	delete[] y;
	delete[] u;
	delete[] u0;
	delete[] v;
	delete[] v0;
	
}







/**
 * \brief Demosaicking chain
 *
 *
 *
 * Compute initialization by Adams-Hamilton algorithm (u0)
 *
 * for h in {16,4,1} do 
 * {
 *
 *		u <- NL_h(u0);			Apply NLmeans demosaicking
 *
 *		u <- CR(u);				Apply chromatic regularization
 *
 *		u0 <- u;
 *
 * }
 *
 * Output <- u;
 *
 *
 * @param[in]  ired, igreen, iblue  initial  image
 * @param[out] ored, ogreen, oblue  filtered output 
 * @param[in]  (redx, redy)  coordinates of the red pixel: (0,0), (0,1), (1,0), (1,1)
 * @param[in]  width, height size of the image
 *
 */



void ssd_demosaicking_chain(int redx,int redy,float *ired,float *igreen,float *iblue,float *ored,float *ogreen,float *oblue,int width,int height)
{
	
	
	////////////////////////////////////////////// Process
	
	

	float h;
	int dbloc = 7;
	float side = 1.5;
	int iter = 1;
	int projflag = 1;  
	float threshold = 2.0;
	
	
	demosaicking_adams(threshold,redx,redy,ired, igreen, iblue, ored, ogreen, oblue, width,height);
	
	

 	h = 16.0;
	demosaicking_nlmeans(dbloc,h,redx,redy,ored,ogreen,oblue,ired,igreen,iblue,width,height);
	chromatic_median(iter,redx,redy,projflag,side,ired,igreen,iblue,ored,ogreen,oblue,width,height);
	
	
	
 	h = 4.0;
 	demosaicking_nlmeans(dbloc,h,redx,redy,ored,ogreen,oblue,ired,igreen,iblue,width,height);
 	chromatic_median(iter,redx,redy,projflag,side,ired,igreen,iblue,ored,ogreen,oblue,width,height);
 	
 	
	
 	h = 1.0;
 	demosaicking_nlmeans(dbloc,h,redx,redy,ored,ogreen,oblue,ired,igreen,iblue,width,height);
 	chromatic_median(iter,redx,redy,projflag,side,ired,igreen,iblue,ored,ogreen,oblue,width,height);
	
	
	
}


