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
* @file borders.cpp
* @brief Mirror symmetry among columns, of B columns on each sides.
* @author Yohann Tendero <yohann.tendero@cmla.ens-cachan.fr>
* @fn void borders(float *Image,float *modified, int w1, int h1, int B)
* @param Image input image
* @param modified output image
* @param w1 width of "image"
* @param h1 height of "image"
* @param B number of columns added on each sides of "Image".
*/

#include "borders.h"

void borders(float *Image,float *modified, int w1, int h1, int B)
{



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////// Copying the image in the midle //////////////////////
////////////////////////////////////////////////////////////////////////////////



    for (int column=0;column<w1;column++)  //for columns in the middle
    {
        for (int line=0;line<h1;line++) //for all lines
        {
            modified[line*(w1+2*B)+B+column]=Image[line*w1+column];

        }
    }




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////// Left side ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



    for (int column=-B;column<0;column++)  //for all columns on the left
    {
        for (int line=0;line<h1;line++)  //for all lines
        {
            modified[line*(w1+2*B)+B+column]=Image[line*w1+(-column)];
        }
    }


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////// Right side //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


    for (int column=w1;column<w1+B;column++) //for all columns on the right
    {
        for (int line=0;line<h1;line++)  //for all lines
        {
            modified[line*(w1+2*B)+B+column]=Image[line*w1+(2*w1-column-1)];
        }
    }
}




