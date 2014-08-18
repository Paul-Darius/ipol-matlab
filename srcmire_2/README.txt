# Readme file for The Midway InfraRed Equalizaton. Basic use :

./demo_MIRE imageIN imageOUT

both imageIN and imageOUT must be png images.
% Midway InfraRed Equalization

# ABOUT

* Author    : This software was written by Yohann Tendero <tendero@cmla.ens-cachan.fr>
* Copyright : (C) 2012, IPOL Image Processing On Line http://www.ipol.im/
* Licence   : GPL v3+, see GPLv3.txt

# OVERVIEW

This source code provides in implementation of the algorithm described at
http://www.ipol.im/pub/algo/glmt_mire/
whose authors are Yohann Tendero, Stephane Landeau and Jerome Gilles.

* the executable file is demo_MIRE

This program reads and write PNG image.

- Compilation. 
Automated compilation requires the make program.

- Library. 
This code requires the libpng librarym and uses the io_png routines written by Nicolas Limare <nicolas.limare@cmla.ens-cachan.fr>
io_png.c is distributed under a GPL3+ or BSD license, at your
option. See the included copyright notice, conditions and disclaimer
for details.

 


# COMPILATION
1. Download the code package and extract it. Go to that directory.

2. Compile the source code (on Unix/Linux/Mac OS).
There are two ways to compile the code.
(1) With Open Multi-Processing multithread parallelization
(http://openmp.org/). Roughly speaking, it accelerates the program using the
multiple processors in the computer. Run
make OMP=1



OR
(2) If the complier does not support OpenMp, run
make

# USAGE

'demo_MIRE' takes 2 parameters: `demo_MIRE in out`

* `in`      : input image
* `out`     : output image

example : ./demo_MIRE test1_8bits.png out.png

#Remark: to perform among lines rotate the input image first. Example (imagemagick) :  convert -rotate 90 IN.png OUT.png

# ABOUT THIS FILE

Copyright 2012 IPOL Image Processing On Line http://www.ipol.im/
Author: This software was written by Yohann Tendero <tendero@cmla.ens-cachan.fr>
Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.

