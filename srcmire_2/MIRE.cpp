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
* @file MIRE.cpp
* @brief Routines using MIRE
*
* @author Yohann Tendero <yohann.tendero@cmla.ens-cachan.fr>
*/

#ifdef _OPENMP
#include <omp.h>
#endif

#include <stdio.h>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
#include "MIRE.h"

#define ABS(x)    (((x) > 0) ? (x) : (-(x)))
#ifndef M_PI
using namespace std;
/**
* M_PI is a POSIX definition for Pi
*/
#define M_PI 3.14159265358979323846
#endif



/**
* @fn void MIRE_automatic(float *IMAGE, int w1, int h1,int SIGMA_MIN, int
* SIGMA_MAX, float DELTA)
* @brief Compute the TV of MIRE-processed image for a set of parameter sigma
* namely (SIGMA_MIN:DELTA:SIGMA_MAX).
* Keep the one that minimizing the TV criterion and sent it back to the main.
* @param IMAGE input
* @param w1 image width
* @param h1 image height
* @param SIGMA_MIN
* @param SIGMA_MAX
* @param DELTA : step between two sigmas
*
*/

void MIRE_automatic(float *IMAGE, int w1, int h1,int SIGMA_MIN, int SIGMA_MAX,
                    float DELTA)
{
/// Arguments : image,  image size, SIGMA_MIN, SIGMA_MAX, DELTA : step between
/// two sigmas
/// The function guess the optimal sigma "sigma_best "and applies a MIRE with
/// "sigma_best"
    float sigma_current;
    float sigma_best;
    float TV_current;
    float* Imtemp = new float[h1*w1];
    float TV_min;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////INITIALIZATION STEP/////////////////////////////
//////////////////////////////////////////////////////////////////////////////

    sigma_best=SIGMA_MIN;
    for (int i=0;i<w1*h1;i++)
        Imtemp[i]=IMAGE[i];


    if (SIGMA_MIN==0)
    {
        TV_current=TV_column_norm(Imtemp,w1,h1,4*SIGMA_MAX);
    }
    else
    {
        TV_current=TV_column_norm(MIRE(Imtemp,SIGMA_MIN,w1,h1),w1,h1,
                                  4*SIGMA_MAX);
    }
    TV_min=TV_current;
    sigma_current=SIGMA_MIN;



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////GUESSING THE BEST SIGMA/////////////////////////
//////////////////////////////////////////////////////////////////////////////



    int T=round(((SIGMA_MAX-SIGMA_MIN)/DELTA))+1;

#pragma omp for
    for ( int i=0;i<T; i++)
    {

        for (int i=0;i<w1*h1;i++)
            Imtemp[i]=IMAGE[i];

        TV_current=TV_column_norm(MIRE(Imtemp,sigma_current,w1,h1),w1,h1,
                                  4*SIGMA_MAX);

        if (TV_current<TV_min)
        {
            sigma_best=sigma_current;
            TV_min=TV_current;
        }
        sigma_current=sigma_current+DELTA;
    }
    delete [] Imtemp;





//////////////////////////////////////////////////////////////////////////////
////////////////APPLYING MIRE WITH THE BEST SIGMA PARAMETER///////////////////
//////////////////////////////////////////////////////////////////////////////
    if (sigma_best!=0) MIRE(IMAGE,sigma_best,w1,h1);
    printf("SIGMA_BEST: %f\n", sigma_best);
}






/**
* @fn float *MIRE(float *IMAGE,float sigma, int w1, int h1)
* @brief Performs the MIRE algorithm with parameter sigma
* @param IMAGE input
* @param sigma
* @param w1 image width
* @param h1 image height
* @return IMAGE
*/

float *MIRE(float *IMAGE,float sigma, int w1, int h1)
{
/// Arguments : image, std-dev of gaussian, image size.
///  output image processed with sdt-dev equal to sigma

    std::vector <std::vector<float> > v;
    std::vector <std::vector<float> > c_sorted;
    c_sorted=column_sorting(IMAGE,w1,h1);  //permits to sorts all columns.
    v=target_histogram(c_sorted,w1,h1,sigma);

    int N=round(4*sigma);
    for (int column=N; column<w1-N;column++)
    {
// avoiding parts added by mirror-symetrization

// v is the target histogram in the sense of a midway weighted histogram
        specify_column(IMAGE,w1,h1,column,v[column-N]);
//Giving the column "column" the histogram v

    }
    return(IMAGE);

}




/**
* @fn float TV_column_norm(float *IMAGE, int w1, int h1,float B)
* @brief Compute TV-norm among colums (avoids the parts added by
symetrization).
*@param IMAGE : input
* @param w1 image width
* @param h1 image height
* @param B number of columns added by symetrization.
* @return TV
*/


float TV_column_norm(float *IMAGE, int w1, int h1,float B)
{
/// Arguments : image, width, length, B: number of columns added by
/// symetrization.
///  Computing the TV-norm among columns avoiding the parts added by
/// symetrization.
    float TV=0;
    for (int column=B;column<w1-B;column++)
    {
        for (int line=0;line<h1;line++)
        {


            TV=TV+ABS((IMAGE[line*w1+column+1])-IMAGE[line*w1+column]);

        }
    }

    return(TV);
}



/**
* @fn float gaussian(int x,float sigma)
* @brief Evaluate the Gaussian function at x with std-dev sigma
* @param x point of evaluation
* @param sigma std-dev of the gaussian
* @return 1/(sigma*sqrt(2*M_PI))*exp(-(x*x)/(2*sigma*sigma))
*/
float gaussian(int x,float sigma)
{
//return gaussian function with std-dev sigma evaluated at x
    return(1/(sigma*sqrt(2*M_PI))*exp(-(x*x)/(2*sigma*sigma)));
}



/**
* @fn void specify_column(float *IMAGE, int w1, int h1,int column_current,
* std::vector <float> target_values)
* @brief Given the vector containing the target value. Specify he column on
* theses values
*  Implemented in 2 steps:
*  Step1: sort v_column put it in column_sorted
*  Step2: for each entry v_column(i) find indice j of column_sorted such that
* column_sorted(j)==v_column(i)
*  change v_column(j) to  v_column(j)=target_values(j)
* @param IMAGE input
* @param w1 image width
* @param h1 image height
* @param column_current the index of the column to process
* @param target_values vector containing the values to apply
*
*/


void specify_column(float *IMAGE, int w1, int h1,int column_current,
                    std::vector <float> target_values)
{
/// given a column (vector) of the image (v_column) an a vector containing
/// target values (target_values) change the values of the image such that
///  the min (c_i):=target_values(0), the next value of c_i changes to
/// target_values(1) and so on
///  Implemented in 2 steps:
///  Step1: sort v_column put it in column_sorted
///  Step2: for each entry v_column(i) find indice j of column_sorted such that
/// column_sorted(j)==v_column(i)
///  change v_column(j) to  v_column(j)=target_values(j)





//////////////////////////////////////////////////////////////////////////////
//////////////////////////////STEP 1//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

    std::vector <float> column_sorted;
    for (int i=0;i<h1;i++)
    {
//for all lines.
        column_sorted.push_back(IMAGE[i*w1+column_current]);
    }
    std::sort (column_sorted.begin(), column_sorted.end());



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////STEP 2//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


    for (int i=0;i<h1;i++)
    {
        float temp= IMAGE[i*w1+column_current] ; //IMAGE[i*w1+column_current]
        //is v_column(i) in the above description
        for (int j=0;j<h1;j++)
        {
            if (temp==column_sorted[j])
            {
                IMAGE[i*w1+column_current]=target_values[j];
            }
        }
    }


}


/**
* @fn std::vector <std::vector<float> >  target_histogram(std::vector
* <std::vector<float> > V_HISTOS,int w1,int h1, float sigma)
* @brief Compute the target vector (~ histogram)
*  Implemented in 3 steps:
*  Step1 : extract columns columns in the interval ["column"-4sigma,
"column"+4sigma]
*  Step2 : Sort all vectors obtained from step1 separately
*  Step3 : for all lines and all column of step2 compute the midway gaussian
averaged ie :
*  v(i)=WEIGHT(column).*(paquet(ligne,column)));
* @param V_HISTOS vector containing sorted columns.
* @param w1 image width
* @param h1 image height
* @param sigma std-dev of the Gaussian.
* @return FINAL
*/
std::vector <std::vector<float> >  target_histogram(std::vector
        <std::vector<float> > V_HISTOS,int w1,int h1, float sigma)
{
/// Compute the midway Gaussian averaged histogram. Gaussian weighted,
/// troncated with radius equal to 4 sigma:
/// only the columns in the interval ["column"-4sigma, "column"+4sigma] are
/// taken into account.
/// Input: image, column current, std-dev sigma
/// Output : Midway-gaussian-averaged histogram (vector).

///  Implemented in 3 steps:
///  Step1 : extract columns columns in the interval ["column"-4sigma,
///  "column"+4sigma]
///  Step2 : Sort all vectors obtained from step1 separately
///  Step3 : for all lines and all column of step2 compute the midway gaussian
///  averaged ie :
/// v(i)=WEIGHT(column).*(paquet(ligne,column)));



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////STEPS 1 & 2/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


    std::vector <std::vector<float> > FINAL;

//Radius in pixels
    int N=round(4*sigma); // (depending on delta) could be non-integer.


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////STEP 3//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


    for (int CENTER=N; CENTER<w1-N;CENTER++)
    {
        std::vector <float> v;
        for (int vline=0;vline<h1;vline++)  //for each line of the "matrix"
        {
            float temp=0;
            for (int vcolumn=CENTER-N; vcolumn<CENTER+N+1;vcolumn++)
            {
                temp=temp+gaussian((CENTER-vcolumn),sigma)
                     *(V_HISTOS[vcolumn][vline]);
            }
            v.push_back(temp);
        }
        FINAL.push_back(v);
    }
    return(FINAL);
}






/**
* @fn std::vector <float> histo_column(float *IMAGE,int w1,int h1, int column)
* @brief Take the column N°column, put it in a vertor and sort it
* @param IMAGE input
* @param w1 image width
* @param h1 image height
* @param column index of the column
* @return v
*/

std::vector <float> histo_column(float *IMAGE,int w1,int h1, int column)
{
///  compute the histogram of the column "column" (v contains  sorted columns )

    std::vector <float> v;
    for (int line=0;line<h1;line++)
    {
        v.push_back(IMAGE[line*w1+column]);
// adding the value for each line of column "column" in the vector
    }
    std::sort (v.begin(), v.end());
    return(v);
}


/**
* @fn std::vector <std::vector<float> >  column_sorting(float *IMAGE,int w1,int
* h1)
* @brief Sort all columns of the image
* @param IMAGE input
* @param w1 image width
* @param h1 image height
* @return V_HISTOS
*/
std::vector <std::vector<float> >  column_sorting(float *IMAGE,int w1,int h1)
{



//Radius in pixels
//int N=round(4*sigma); // (depending on delta) could be non-integer.

    std::vector <std::vector<float> > V_HISTOS;
// Vectors of vectors (matrix) such that
//V_HISTOS[i] is the histogram of the column v (which is also a vector)
    for (int i=0;i <w1;i++)   //processing all columns in the radius
    {
        V_HISTOS.push_back(histo_column(IMAGE, w1, h1,  i));
//step 2 is done in histo_column
    }
    return(V_HISTOS);
}





