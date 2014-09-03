function out=mosaic(image,string)
% MOSAIC returns a Bayern pattern mosaic from the input image.
%   out = MOSAIC(image,string) returns the pattern.
%
% INPUT 1 :
% =========
%
% The image must be a double matrix.
% To load it from an image on your computer use the following command :
% image=im2double(imread('adress/of/file.pgm'))
%
% INPUT 2 :
% =========
%
% The string must be 'RGGB', 'GRBG', 'GBRG' or 'BGGR'.
% It depends on the needed pattern.
%
% OUTPUT :
% ========
%
% The output is a double matrix.
% It contains the Bayern pattern mosaic.
%
% You can use the output as a test for the demosaicking algorithm.
% Type "help ssdd" for more informations.
%
% More informations at : http://www.ipol.im/pub/art/2011/bcms-ssdd

out=MEX_mosaic(image,string);