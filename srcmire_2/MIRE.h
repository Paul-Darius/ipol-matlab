/*  MIRE.cpp */
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


#include <vector>
float *MIRE( float [],float,int,int);
//works on a copy of the image
//(it's unsure wich sigma is the right one a first sight)
/// Arguments : image, std-dev of the gaussian, image size.
void MIRE_automatic(float [],int, int,int,int,float);
/// Arguments : image, image size, sigma_min,simga_max,sigma_step:
//all sigma_min:sigma_step:sigma_max will be tested (Matlab notation).
float TV_column_norm(float [],int,int,float);
/// Arguments : image, image size.

void specify_column(float [], int , int ,int , std::vector <float> );
/// Arguements : imge, image size, column to be processed, target values.

float gaussian(int ,float );
/// Arguements : position (in pixel), std-dev.

std::vector <std::vector<float> > target_histogram(
    std::vector <std::vector<float> > ,int, int , float );
/// Arguments : vector of vector containing the sorted values, image size,sigma.

std::vector <std::vector<float> >  column_sorting(float [],int ,int );
/// Arguments : image, image size.

std::vector <float> histo_column(float [],int ,int , int );
/// Arguments : image, image size, #column to be processed.
