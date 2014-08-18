/*
* Copyright 2012 IPOL Image Processing On Line http://www.ipol.im/
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

/**
* @mainpage Midway InfraRed Equalization (MIRE)
*
* README.txt:
* @verbinclude README.txt
*/

/**
* @file demo_MIRE.cpp
* @brief Columns-artifacts removal main file
*
* The input  is 8bits png corrupted with non-unifomity (columns artifacts).
* The  is processed with a Gaussian-weighted local Midway algorithm.
* The output is 8bits png
*
* @author Yohann Tendero <yohann.tendero@cmla.ens-cachan.fr>
*/
#ifdef _OPENMP
#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "io_png/io_png.h"
#include "MIRE.h"
#include "borders.h"

/**
* @fn main(int argc, char **argv)
* @brief main function
* @param argc
* @param **argv : ImNoisy  Noisy (corrupted), gray-level (1 channel) PNG image.
* ImDenoised denoised  PNG image.
*
*/
int main(int argc, char **argv)
{
//Check arguments : IN OUT;
    if (argc != 3)
    {
        std::cerr << " **************************************** " << std::endl
        << " **********  MIRE  ******************************** " << std::endl
        << " ************************************************** " << std::endl
        << "Usage: " << argv[0] << " ImNoisy.png ImDenoised.png " << std::endl
        << "Input" << std::endl
        << "ImNoisy: columns artifacts, gray (1 channel),  PNG. " << std::endl
        << "Output" << std::endl
        << "ImDenoised: denoised  in PNG. " << std::endl
        << " ************************************************** " << std::endl
        << "****************  Yohann Tendero, 2011  *********** " << std::endl
        << " ************************************************** " << std::endl;
        return 1;
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////// CONSTANT PARAMETER DEFINITION////////////////////////
////////////////////////////////////////////////////////////////////////////////

//During the optimal sigma parameter guess phase each sigma such that
//sigma=SIGMA_MIN+k*DELTAT and sigma<=SIGMA_MAX will be tested.

    const int SIGMA_MIN=0; // minimal std-dev of the Gaussian-weighting function
    const int SIGMA_MAX=8; //maximal std-dev of the Gaussian-weighting function
    const float DELTA=0.5; //step between two consecutive std-dev




////////////////////////////////////////////////////////////////////////////////
////////////////////////// READ IMAGE///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    float * Image;  //image
    size_t w1, h1; // width an height of the image
    if (NULL == (Image = read_png_f32_gray(argv[1], &w1, &h1)))
    {
        std::cerr << "Unable to load  file " << argv[1] << std::endl;
        return 1;
    }



////////////////////////////////////////////////////////////////////////////////
//////////////DEALING WITH BORDER EFFECTS: MIRROR symmetry ON COLUMNS////////////
////////////////////////////////////////////////////////////////////////////////




    int W=w1+8*SIGMA_MAX;
//new  width of image afer mirror symmetry : 4 times SIGMA_MAX on each sides
    float* Imsym= new float[W*h1]; //new image allocation

//input : image, New image, width,height, total size of the extension
    borders(Image,Imsym,w1,h1,4*SIGMA_MAX); //Output :
//Classic mirror symmetry on columns :
//    C1 C2 ... CN => C2 C1 |C1 C2 ... CN|CN CN-1 etc





////////////////////////////////////////////////////////////////////////////////
//////////////////////////TRANSFERING DATAS TO CENTRAL FUNCTION/////////////////
////////////////////////////////////////////////////////////////////////////////

//input image , width (after symetrication), height
    MIRE_automatic(Imsym,W,h1,SIGMA_MIN, SIGMA_MAX, DELTA);




////////////////////////////////////////////////////////////////////////////////
///////////REMOVING THE SYMETRIZATION : CROP (see DEALING WITH BORDERS//////////
////////////////////////////////////////////////////////////////////////////////



//#pragma omp parallel for
// The following realize a CROP
    for (unsigned column=0;column<w1;column++)  // for all columns
    {
        for (unsigned line=0;line<h1;line++)  // for all lines
        {
            Image[line*w1+column]
            =Imsym[line*(w1+8*SIGMA_MAX)+column+4*SIGMA_MAX];
// using the useless memory of the input
        }
    }

    delete [] Imsym;



////////////////////////////////////////////////////////////////////////////////
//////////////////////////////DYNAMIC CORRECTION////////////////////////////////
///////////////////////////////IMPOSING/////////////////////////////////////////
///////////////////////////////[0,255]//////////////////////////////////////////


//Computing the min and max of the output
    float min=Image[0];
    float max=Image[0];
//#pragma omp parallel for
    for (unsigned i=1;i<w1*h1;i++)
    {
        if (Image[i]<min) min=Image[i];
        if (Image[i]>max) max=Image[i];
    }


//Actually changing image-values
//#pragma omp parallel for
    for (unsigned i=1;i<w1*h1;i++)
    {
        Image[i]=(255*(Image[i]-min)/(max-min));
    }


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////WRITING THE OUTPUT////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    write_png_f32(argv[2],Image,w1, h1, 1);
    free(Image);


    return 0;
}
