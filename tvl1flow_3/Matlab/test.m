
a=double(imread('a.png'));
b=double(imread('b.png'));
sz = size(a);

% ONLY ONE CHANNEL!!!
a=mean(a,3);
b=mean(b,3);

out=MEX_main(a,b);

% display the flow
imagesc(flowToColor(out))

% back-flow
[X,Y] = meshgrid(1:sz(2), 1:sz(1));
back = interp2(b,X+out(:,:,1),Y+out(:,:,2));
figure; 
imagesc(abs(back-a))
