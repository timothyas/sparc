function [] = readMat()


N = 4039;
fname1='matrix.dat';
fname2='newMatrix.dat';



fid=fopen(fname1,'r');
mat1 = fscanf(fid,'%d');
fclose(fid);
fid=fopen(fname2,'r');
mat2 = fscanf(fid,'%d');
fclose(fid);

mat1=reshape(mat1,[N N]);
mat2=reshape(mat2,[N N]);

figure; spy(mat1)
figure; spy(mat2)

keyboard

end
