X=double(imread('input.png'));
%X=floor(256*X/max(max(max(X))));% IL FAUT QUE L'IMAGE SOIT ENTRE 0 ET 255(pas 0 et 1)
% IL FAUT QUE L'IMAGE SOIT EN NIVEAU DE GRIS
% Vérifier que des inputs un peu aléatoires ne font pas planter le soft(en
% entrée, une image en couleur double(ie NxMx3) fait planter le soft)
output_lsd=lsd(X);
figure,
hold on
imagesc(X);
colormap gray;
for i=1:size(output_lsd,1)
    plot([output_lsd(i,1),output_lsd(i,3)],[output_lsd(i,2),output_lsd(i,4)]);
end
hold off