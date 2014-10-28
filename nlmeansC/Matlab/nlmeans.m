function (out1,out2)=nlmeans(A,fSigma)
% NLMEANS returns two images. The first one is a noisy version of A and the second one is denoised.
%   (out1,out2)=nlmeans(A,fSigma) returns the two images
%
% INPUT :
% =======
%
% A must be a double matrix.
% To load it from an image on your computer -which has to be a .pgm file- use the following command :
% A=double(imread('adress/of/file.pgm'))
%
% fSigma must be a number between 0 and 100. The highest is fSigma, the noisier is the image.
%
%   RETURNED VALUES :
%   ================
%
%   The returned values are two images.
%   The first one contains the noisy image.
%   The second one contains the denoised ımage.
%   http://www.ipol.im/pub/art/2012/gjmr-lsd/

(out1,out2)=MEX_nlmeans(A,fSigma)
