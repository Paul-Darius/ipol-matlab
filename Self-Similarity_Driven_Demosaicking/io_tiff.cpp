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


#include "io_tiff.h"



/*
 * READ FUNCTIONS
 */

/**
 * @brief load the raw data from a TIFF image file as an uint32 array.
 *
 * The array is allocated by this function. Uint32 is the native data
 * type in libtiff.
 *
 * @param fname the file name to read
 * @param nx, ny storage space for the image size
 *
 * @return the data array pointer, NULL if an error occured
 */
static uint32 *read_tiff_rgba_raw(const char *fname, size_t * nx, size_t * ny)
{
    TIFF *tiffp = NULL;
    uint32 width = 0, height = 0;
    uint32 *data_tiff = NULL;
    size_t size;
	
    /* no warning messages */
    (void) TIFFSetWarningHandler(NULL);
	
    /* open the TIFF file and structure */
    if (NULL == (tiffp = TIFFOpen(fname, "r")))
        return NULL;
	
    /* read width and height and allocate the storage raster */
    if (1 != TIFFGetField(tiffp, TIFFTAG_IMAGEWIDTH, &width)
        || 1 != TIFFGetField(tiffp, TIFFTAG_IMAGELENGTH, &height)
        || NULL == (data_tiff = (uint32 *) malloc(width * height
                                                  * sizeof(uint32))))
    {
        TIFFClose(tiffp);
        return NULL;
    }
	
    size = (size_t) width * (size_t) height;
    if (NULL != nx)
        *nx = (size_t) width;
    if (NULL != ny)
        *ny = (size_t) height;
	
    /* read the image data */
    if (1 != TIFFReadRGBAImageOriented(tiffp, width, height, data_tiff,
                                       ORIENTATION_TOPLEFT, 1))
    {
        free(data_tiff);
        TIFFClose(tiffp);
        return NULL;
    }
	
    /* close the TIFF file and structure */
    TIFFClose(tiffp);
	
    return data_tiff;
}

/**
 * @brief load the data from a TIFF image file as an RGBA array
 *
 * The array is allocated by this function. It hold the RGBA pixels as
 * 4 contiguous arrays.
 *
 * @param fname the file name to read
 * @param nx, ny storage space for the image size
 * @param dtype the desired output type : DT_U8 or DT_F32
 *
 * @return the data array pointer, NULL if an error occured
 */
static void *read_tiff_rgba(const char *fname, size_t * nx, size_t * ny,
							int dtype)
{
    uint32 *data_tiff = NULL;
    uint32 *ptr_in, *ptr_end;
	
    /* get the raw TIFF data */
    if (NULL == (data_tiff = read_tiff_rgba_raw(fname, nx, ny)))
        return NULL;
    ptr_in = data_tiff;
    ptr_end = ptr_in + *nx * *ny;
	
    switch (dtype)
    {
		case DT_U8:
		{
			unsigned char *data = NULL;
			unsigned char *ptr_r, *ptr_g, *ptr_b, *ptr_a;
			
			/* allocate the RGBAarray */
			if (NULL == (data = (unsigned char *) malloc(4 * *nx * *ny 
														 * sizeof(unsigned char))))
			{
				free(data_tiff);
				return NULL;
			}
			
			/* setup the pointers */
			ptr_r = data;
			ptr_g = ptr_r + *nx * *ny;
			ptr_b = ptr_g + *nx * *ny;
			ptr_a = ptr_b + *nx * *ny;
			
			/*
			 * deinterlace the TIFF raw array (ptr_in)
			 * into four arrays (ptr_r, ptr_g, ptr_b, ptr_a)
			 */
			while (ptr_in < ptr_end)
			{
				*ptr_r++ = (unsigned char) TIFFGetR(*ptr_in);
				*ptr_g++ = (unsigned char) TIFFGetG(*ptr_in);
				*ptr_b++ = (unsigned char) TIFFGetB(*ptr_in);
				*ptr_a++ = (unsigned char) TIFFGetA(*ptr_in);
				ptr_in++;
			}
			
			free(data_tiff);
			
			return (void *) data;
		}
		case DT_F32:
		{
			float *data = NULL;
			float *ptr_r, *ptr_g, *ptr_b, *ptr_a;
			
			/* allocate the RGBAarray */
			if (NULL == (data = (float *) malloc(4 * *nx * *ny 
												 * sizeof(float))))
			{
				free(data_tiff);
				return NULL;
			}
			
			/* setup the pointers */
			ptr_r = data;
			ptr_g = ptr_r + *nx * *ny;
			ptr_b = ptr_g + *nx * *ny;
			ptr_a = ptr_b + *nx * *ny;
			
			/*
			 * deinterlace the TIFF raw array (ptr_in)
			 * into four arrays (ptr_r, ptr_g, ptr_b, ptr_a)
			 */
			while (ptr_in < ptr_end)
			{
				*ptr_r++ = (float) TIFFGetR(*ptr_in);
				*ptr_g++ = (float) TIFFGetG(*ptr_in);
				*ptr_b++ = (float) TIFFGetB(*ptr_in);
				*ptr_a++ = (float) TIFFGetA(*ptr_in);
				ptr_in++;
			}
			
			free(data_tiff);
			
			return (void *) data;
		}
		default:
			return NULL;
    }
}

/**
 * @brief load the data from a TIFF image file as a float RGBA array
 *
 * The array is allocated in this function call. It hold the RGBA
 * pixels as 4 contiguous arrays.
 *
 * @param fname the file name to read
 * @param nx, ny storage space for the image size
 *
 * @return the data array pointer, NULL if an error occured
 */
float *read_tiff_rgba_f32(const char *fname, size_t * nx, size_t * ny)
{
    return (float *) read_tiff_rgba(fname, nx, ny, DT_F32);
}

/**
 * @brief load the data from a TIFF image file as a 8bit RGBA array
 *
 * The array is allocated in this function call. It hold the RGBA
 * pixels as 4 contiguous arrays.
 *
 * @param fname the file name to read
 * @param nx, ny storage space for the image size
 *
 * @return the data array pointer, NULL if an error occured
 */
unsigned char *read_tiff_rgba_u8(const char *fname, size_t * nx, size_t * ny)
{
    return (unsigned char *) read_tiff_rgba(fname, nx, ny, DT_U8);
}

/*
 * WRITE FUNCTIONS
 */

/**
 * @brief save an array into a LZW-compressed TIFF file
 *
 * @return 0 if OK, != 0 if an error occured
 */
static int write_tiff_rgba_raw(const char *fname, const uint32 *data_tiff,
							   size_t nx, size_t ny)
{
    TIFF *tiffp = NULL;
    uint16 alpha_type[] = { EXTRASAMPLE_ASSOCALPHA };
    size_t size;
	
    /*
     * ensure the data is allocated
     * and the width and height are within the limits
     * (tiff uses uint32, 2^32 - 1 = 4294967295)
     * and open the TIFF file and structure
     */
    if (NULL == data_tiff
        || 4294967295. < (double) nx || 4294967295. < (double) ny)
        return -1;
    size = nx * ny;
	
    /* no warning messages */
    (void) TIFFSetWarningHandler(NULL);
	
    /* open the TIFF file and structure */
    if (NULL == (tiffp = TIFFOpen(fname, "w")))
        return -1;
	
    /* insert tags into the TIFF structure */
    if (1 != TIFFSetField(tiffp, TIFFTAG_IMAGEWIDTH, nx)
        || 1 != TIFFSetField(tiffp, TIFFTAG_IMAGELENGTH, ny)
        || 1 != TIFFSetField(tiffp, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT)
        || 1 != TIFFSetField(tiffp, TIFFTAG_BITSPERSAMPLE, 8)
        || 1 != TIFFSetField(tiffp, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG)
        || 1 != TIFFSetField(tiffp, TIFFTAG_ROWSPERSTRIP, ny)
        || 1 != TIFFSetField(tiffp, TIFFTAG_SAMPLESPERPIXEL, 4)
        || 1 != TIFFSetField(tiffp, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB)
        || 1 != TIFFSetField(tiffp, TIFFTAG_COMPRESSION, COMPRESSION_LZW)
        || 1 != TIFFSetField(tiffp, TIFFTAG_EXTRASAMPLES, 1, alpha_type))
    {
        TIFFClose(tiffp);
        return -1;
    }
	
    /* write the image as one single strip */
    if (4 * nx * ny != (unsigned int)
        TIFFWriteEncodedStrip(tiffp, (tstrip_t) 0, (tdata_t) data_tiff,
                              (tsize_t) (4 * size)))
    {
        TIFFClose(tiffp);
        return -1;
    }
	
    /* free the TIFF structure and data, return success */
    TIFFClose(tiffp);
    return 0;
}

/**
 * Macro for packing RGBA components to the ABGR form used by
 * TIFFReadRGBAImage.
 *
 * See TIFFGetR() in tiffio.h for the reverse operation.
 */
#define TIFFPackRGBA(r, g, b, a) (uint32) ((r & 0xff)                   \
| (g & 0xff) << 8            \
| (b & 0xff) << 16           \
| (a & 0xff) << 24)

/**
 * @brief save four contiguous arrays into a TIFF file
 *
 * The array balues are supposed in [0,255].
 * 
 * @param fname TIFF file name
 * @param data input array of float values supposed in [0,255]
 * @param nx ny array size
 * @param dtype the input data type : DT_U8 or DT_F32
 *
 * @return 0 if OK, != 0 if an error occured
 */
static int write_tiff_rgba(const char *fname, const void *data,
						   size_t nx, size_t ny, int dtype)
{
    uint32 *data_tiff = NULL;
    uint32 *ptr_out, *ptr_end;
    int retval;
	
    /* check allocaton */
    if (NULL == data)
        return -1;
	
    /* create the tiff array */
    if (NULL == (data_tiff = (uint32*) malloc(nx * ny * sizeof(uint32))))
        return -1;
    ptr_out = data_tiff;
    ptr_end = ptr_out + nx * ny;
	
    switch (dtype)
    {
		case DT_U8:
		{
			const unsigned char *ptr_r, *ptr_g, *ptr_b, *ptr_a;
			
			/* setup the pointers */
			ptr_r = (unsigned char *) data;
			ptr_g = ptr_r + nx * ny;
			ptr_b = ptr_g + nx * ny;
			ptr_a = ptr_b + nx * ny;
			
			/*
			 * interlace four arrays (ptr_r, ptr_g, ptr_b, ptr_a)
			 * into the TIFF raw array (ptr_out)
			 */
			while (ptr_out < ptr_end)
				*ptr_out++ = TIFFPackRGBA((uint8) (*ptr_r++),
										  (uint8) (*ptr_g++),
										  (uint8) (*ptr_b++),
										  (uint8) (*ptr_a++));
			break;
		}
		case DT_F32:
		{
			const float *ptr_r, *ptr_g, *ptr_b, *ptr_a;
			
			/* setup the pointers */
			ptr_r = (float *) data;
			ptr_g = ptr_r + nx * ny;
			ptr_b = ptr_g + nx * ny;
			ptr_a = ptr_b + nx * ny;
			
			/*
			 * interlace four arrays (ptr_r, ptr_g, ptr_b, ptr_a)
			 * into the TIFF raw array (ptr_out)
			 */
			while (ptr_out < ptr_end)
				*ptr_out++ = TIFFPackRGBA((uint8) (*ptr_r++ + .5),
										  (uint8) (*ptr_g++ + .5),
										  (uint8) (*ptr_b++ + .5),
										  (uint8) (*ptr_a++ + .5));
			break;
		}
		default:
			return -1;
    }
	
    /* write the file */
    retval = write_tiff_rgba_raw(fname, data_tiff, nx, ny);
	
    free(data_tiff);
	
    return retval;
}

/**
 * @brief save four contiguous float arrays into a TIFF file
 *
 * @param fname TIFF file name
 * @param data input array of float values supposed in [0,255]
 * @param nx ny array size
 *
 * @return 0 if OK, != 0 if an error occured
 */
int write_tiff_rgba_f32(const char *fname, const float *data,
						size_t nx, size_t ny)
{
    return write_tiff_rgba(fname, (void *) data, nx, ny, DT_F32);
}

/**
 * @brief save four contiguous 8bit arrays into a TIFF file
 *
 * @param fname TIFF file name
 * @param data input array of 8bit values
 * @param nx ny array size
 *
 * @return 0 if OK, != 0 if an error occured
 */
int write_tiff_rgba_u8(const char *fname, const unsigned char *data,
					   size_t nx, size_t ny)
{
    return write_tiff_rgba(fname, (void *) data, nx, ny, DT_U8);
}


