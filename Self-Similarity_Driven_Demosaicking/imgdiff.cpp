/*
 * Copyright 2009, 2010 IPOL Image Processing On Line 
 *  <http://www.ipol.im/>
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
 * @file imgdiff.cpp
 * @brief simple tool to visualize the difference between two images
 *
 * The difference visialization for a 8bit channel is
 * (B - A) * 128 / D + 128, ie the visu value is 128 when there is no
 * difference between the two images, and [0, 255] for differences in
 * the [-D, D] interval.
 *
 * @author Nicolas Limare <nicolas.limare@cmla.ens-cachan.fr>,  Antoni Buades <toni.buades@uib.es>
 *
 * @todo difference between RGB and grayscale
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "io_tiff.h"

/**
 * @brief mail program call
 */
int main(int argc, char **argv)
{
	
    size_t nx, ny;
    size_t _nx, _ny;
    float *data, *data2;
    float *data_ptr, *data2_ptr;
	float delta;
    float scale, tmp;
	int i, c;
	float sdif[3];
	
    /* "-v" option : version info */
    if (2 <= argc && 0 == strcmp("-v", argv[1]))
    {
        fprintf(stdout, "%s version " __DATE__ "\n", argv[0]);
        return EXIT_SUCCESS;
    }
    /* wrong number of parameters : simple help info */
    if (5 != argc)
    {
        fprintf(stderr, "usage : %s A.tiff B.tiff D out.tiff\n",
		argv[0]);
        fprintf(stderr, "        D : max difference to visualize\n");
        return EXIT_FAILURE;
    }

    /* rgb sigma parameters */
    delta = atof(argv[3]);
    if (0 >= delta)
    {
        fprintf(stderr, "delta must be > 0\n");
        return EXIT_FAILURE;
    }

    /* read the two images */
    data = (float *) read_tiff_rgba_f32(argv[1], &nx, &ny);
    data2 = (float *) read_tiff_rgba_f32(argv[2], &_nx, &_ny);

    /* check the image sizes */
    if ((nx != _nx) || (ny != _ny))
    {
        fprintf(stderr, "image sizes don't match\n");
        free(data);
        free(data2);
        return EXIT_FAILURE;
    }

	
	
	
    /* compute the difference */
	{
		
		data_ptr = data;
		data2_ptr = data2;
		scale = 128. / delta;
		
		for (c=0; c < 3; c++) 
		{
			
			i = 0;
			sdif[c] = 0.0f;
		
			while (i < nx * ny)
			{
				
				/* data -= data2 */
				*data_ptr -= *data2_ptr++;
				
				sdif[c]  +=  *data_ptr * (*data_ptr);
				
				/* data *= 128 / delta */
				/* differences > delta are out out the [-128,128] interval */
				*data_ptr *= scale;
				/* data += 128 */
				*data_ptr += 128.;
				/* round the values to the closest integer,
				 * and truncate to [0..255] */
				tmp = floor(*data_ptr + .5);
				*data_ptr++ = (tmp > 255 ? 255. : (tmp < 0 ? 0. : tmp));
				
				i++;
			}
			
			sdif[c] /= (float) (nx*ny);
			sdif[c] = sqrt(sdif[c]);
			
		}
		
		printf("Root Mean Square Error (R,G,B) = (%2.2f, %2.2f, %2.2f)\n", sdif[0], sdif[1], sdif[2]);

	}
	
	

    /* save the result */
    write_tiff_rgba_f32(argv[4], data, nx, ny);

    free(data);
    free(data2);

    return EXIT_SUCCESS;
}
