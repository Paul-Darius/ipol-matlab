function out=mire(A)
% MIRE returns an image containing the mire algorithm applied to A.
%   out = MIRE(A) returns the mire algorithm applied to image
%
% INPUT :
% =======
%
% A must be a double matrix.
% To load it from an image on your computer use the following command :
% A=im2double(imread('adress/of/file.pgm'))
%
% RETURNED VALUE :
% ================
%
% The returned value is a double matrix containing the mire algorithm applied to A.
% http://www.ipol.im/pub/art/2012/glmt-mire/

if(~ismatrix(A))
A=rgb2gray(A);
end

out=MEX_demo_MIRE(A);