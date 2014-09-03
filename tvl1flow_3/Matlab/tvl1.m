function out=tvl1(A,B,option)
% TVL1 returns an image containing the optical flow constructed from two images A and B
% out = TVL1(A,B) returns the image
%
% INPUT :
% =======
%
% A and B must be  double matrix.
% To load A or B from an image on your computer use the following command :
% A=im2double(imread('adress/of/file.pgm'))
% or
% B=im2double(imread('adress/of/file.pgm'))
%
% OPTIONS :
% =========
%
% You can change the default options as you wish.
% Just use a structure containing them or the ones you need.
% For example, if you want a scale equals to one and a sigma_coef equals to 0.7, type the following command :
%   option.scale=1;
%   option.sigma_coef=0.8;
%   out = LSD(A,option)
%
% You can name your structure as you want. However its components have to respect some rules :
%
% SCALE :
%   name as a component of the optional structure : scale
%   type : double
%   default value : 0.8
%   minimum value : 0.0
%   maximum value : -
%   Scale image by Gaussian filter before processing.
%
% RETURNED VALUE :
% ================
%
% The returned value is the optical flow found from A and B.
% More informations at :
% http://www.ipol.im/pub/art/2013/26/

if (~exist('option', 'var'))
out=MEX_main(A,B);
else
out=MEX_main(A,B,option);
end