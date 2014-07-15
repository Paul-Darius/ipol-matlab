function out=lsd(image,option)
% LSD  return an array containing datas concerning line segments detected
% in an image
%   out = LSD(A) returns line segments belonging to A
%
% Note : A must be a double matrix
    if (~exist('option', 'var'))
        out=MEX_lsd_cmd(image);
    else
        out=MEX_lsd_cmd(image,option);
    end