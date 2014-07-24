function out=ssdd(image,string)
% SSDD reconstructs an image from a mosaic.
%   out = SSDD(image,string) returns the reconstruction.
%
% INPUT 1 :
% =========
%
% The image must be a double matrix.
% To load it from an image on your computer use the following command :
% image=double(imread('adress/of/file.pgm'))
%
% INPUT 2 :
% =========
%
% The string must be 'RGGB', 'GRBG', 'GBRG' or 'BGGR'.
% It depends on the pattern the mosaic is made from.
%
% OUTPUT :
% ========
%
% The output is a double matrix.
% It contains the reconstruction.
%
% Type "help ssdd" for more informations.
%
% More informations at : http://www.ipol.im/pub/art/2011/bcms-ssdd

out=MEX_demosaic(image,string);
