/*
 * Copyright 2009, 2010 IPOL Image Processing On Line 
 * <http://www.ipol.im/>
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
 * @file mosaic.c
 * @brief extract a Bayer mosaic
 *
 * @author Nicolas Limare <nicolas.limare@cmla.ens-cachan.fr>
 *
 * This program reads a TIFF RGB image, extracts a Bayer mosaic from
 * this image and writes it to a TIFF image.
 *
 * Four patterns are possible, indicated by the RGGB, GRBG, GBRG and
 * BGGR keywords.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_tiff.h"

/**
 * @brief main function call
 */
int main(int argc, char **argv)
{
    size_t nx = 0, ny = 0;
    float *data;
    float *channel;
    int mask_R[4] = {0, 0, 0, 0};
    int mask_G[4] = {0, 0, 0, 0};
    int mask_B[4] = {0, 0, 0, 0};
    size_t i, j;

    /* version info */
    if (2 <= argc && 0 == strcmp("-v", argv[1]))
    {
        fprintf(stdout, "%s version " __DATE__ "\n", argv[0]);
        return EXIT_SUCCESS;
    }
    /* sanity check */
    if (4 != argc)
    {
        fprintf(stderr, "usage : %s input.tiff output.tiff pattern\n",
		argv[0]);
        return EXIT_FAILURE;
    }

    /* get the pattern */
    if (0 == strcmp("RGGB", argv[3]))
    {
	mask_R[0] = 1;
	mask_G[1] = 1;
	mask_G[2] = 1;
	mask_B[3] = 1;
    }
    else if (0 == strcmp("GRBG", argv[3]))
    {
	mask_G[0] = 1;
	mask_R[1] = 1;
	mask_B[2] = 1;
	mask_G[3] = 1;
    }
    else if (0 == strcmp("GBRG", argv[3]))
    {
	mask_G[0] = 1;
	mask_B[1] = 1;
	mask_R[2] = 1;
	mask_G[3] = 1;
    }
    else if (0 == strcmp("BGGR", argv[3]))
    {
	mask_B[0] = 1;
	mask_G[1] = 1;
	mask_G[2] = 1;
	mask_R[3] = 1;
    }
    else
    {
        fprintf(stderr, "pattern must be RGGB, GRBG, GBRG or BGGR\n");
        return EXIT_FAILURE;
    }

    /* input */
    if (NULL == (data = read_tiff_rgba_f32(argv[1], &nx, &ny)))
    {
        fprintf(stderr, "error while reading from %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    /*
     * process the RGB channels by multiplication the input pixel
     * values by a 2x2 mask
     */

    channel = data;
    /* red channel */
    for (j = 0; j < ny; j++)
	for (i = 0; i < nx; i++)
	    channel[i + nx * j] *= mask_R[(i % 2) + 2 * (j % 2)];

    /* green channel */
    channel = data + nx * ny;
    for (j = 0; j < ny; j++)
	for (i = 0; i < nx; i++)
	    channel[i + nx * j] *= mask_G[(i % 2) + 2 * (j % 2)];

    /* blue channel */
    channel = data + 2 * nx * ny;
    for (j = 0; j < ny; j++)
	for (i = 0; i < nx; i++)
	    channel[i + nx * j] *= mask_B[(i % 2) + 2 * (j % 2)];

    /* output */
    write_tiff_rgba_f32(argv[2], data, nx, ny);

    free(data);

    return EXIT_SUCCESS;
}
