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
 * @file io_tiff.cpp
 * @brief TIFF I/O routines
 *
 * @author Nicolas Limare <nicolas.limare@cmla.ens-cachan.fr>
 *
 * @todo stdin/stdout handling
 * @todo simple RGB/grayscale version
 * @todo TIFF float version
 *
 * These routines focus on RGBA 8bit TIFF files. These files can be
 * read into or written from unsigned char or float arrays.
 */

#include <stdlib.h>

#include <tiffio.h>

/** internal data type shortcut for unsigned char */
#define DT_U8  0x01
/** internal data type shortcut for float */
#define DT_F32 0x02





float *read_tiff_rgba_f32(const char *fname, size_t *nx, size_t *ny);
unsigned char *read_tiff_rgba_u8(const char *fname, size_t *nx, size_t *ny);
int write_tiff_rgba_f32(const char *fname, const float *data, size_t nx, size_t ny);
int write_tiff_rgba_u8(const char *fname, const unsigned char *data, size_t nx, size_t ny);




