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
% For example, if you want tau being equals to 0.1 and a lambda equals to 0.7, type the following command :
%   option.tau=0.1;
%   option.lambda=0.7;
%   out = TVL1(A,B,option)
%
% You can name your structure as you want. However its components have to respect some rules :
%
% TAU :
%   name as a component of the optional structure : tau
%   type : double
%   default value : 0.25
%   minimum value : 0.0
%   maximum value : 0.25
%   Time step in the numerical scheme
%
% LAMBDA :
%   name as a component of the optional structure : lambda
%   type : double
%   default value : 0.15
%   minimum value : 0.0
%   maximum value : -
%   Data term weight parameter
%
% THETA :
%   name as a component of the optional structure : theta
%   type : double
%   default value : 0.3
%   minimum value : 0.0
%   maximum value : -
%   Tightness parameter
%
% NSCALES :
%   name as a component of the optional structure : nscales
%   type : double
%   default value : 100
%   minimum value : 0.0
%   maximum value : -
%   Number of scales in the pyramidal structure
%
% ZFACTOR :
%   name as a component of the optional structure : zfactor
%   type : double
%   default value : 0.5
%   minimum value : 0.0
%   maximum value : -
%   Downsampling factor for creating the scales
%
% NWARPS :
%   name as a component of the optional structure : nwarps
%   type : double
%   default value : 5
%   minimum value : 0.0
%   maximum value : -
%   Number of warps per scales
%
% EPSILON :
%   name as a component of the optional structure : epsilon
%   type : double
%   default value : 0.01
%   minimum value : 0.0
%   maximum value : -
%   Stopping criterion threshold for the iterative process
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