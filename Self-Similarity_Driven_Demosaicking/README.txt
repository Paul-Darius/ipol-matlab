SELF SIMILARITY DRIVEN DEMOSAICKING

# ABOUT

This software is written by A. Buades <toni.buades@uib.es>
with contributions from Nicolas Limare.

- demosaickingIpol.cpp, libdemosaicking.cpp and libdemosaicking.h
may be linked to the US patent 5,629,734 by J. Hamilton Jr and 
J. Adams Jr, to the US patent 4,642,678 by  D. Cok and to the
EP patent 1,749,278 by A. Buades, T. Coll and J.M. Morel.
They are provided for scientific and education only.

- All the other files are distributed under the terms of the
  LGPLv3 license.


# OVERVIEW

This source code provides an implementation of the Self Similar 
demosaicking algorithm, as described in IPOL
  http://www.ipol.im/pub/algo/bcms_self_similarity_driven_demosaicking/

Three programs are provided:

* 'demosaickingIpol' reads a tiff image and a CFA configuration, 
and interpolates missing values of the CFA.

* 'mosaic' creates a mosaicked image from an input image and a given CFA
configuration

* 'imgdiff' computes the difference image between original and demosaicked images


# REQUIREMENTS

The code is written in ANSI C, and should compile on any system with
an ANSI C compiler.

The libtiff header and libraries are required on the system for
compilation and execution. 



# COMPILATION

Simply use the provided makefile, with the command `make`.



# USAGE

mosaic input.tiff output.tiff pattern

* `input.tiff`  :  input image
* `output.tiff` :  output image
* `pattern`     :  CFA configuration, pattern must be RGGB, GRBG, GBRG or BGGR


demosaickingIpol input.tiff output.tiff pattern

* `input.tiff`  :  input image
* `output.tiff` :  output image
* `pattern`     :  CFA configuration, pattern must be RGGB, GRBG, GBRG or BGGR


imgdiff input1.tiff input2.tiff D output.tiff

* `input1.tiff`  :  input image 1
* `input2.tiff`  :  input image 2
* `output.tiff`  :  output image (difference image)
*  D             :  maximum difference to visualize


