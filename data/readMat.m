function [] = readMat()


N = 4039;
fname='Matrix.dat';


fid=fopen(fname,'r');
mat = fscanf(fid,'%d');
fclose(fid);

mat=reshape(mat,[N N]);

imagesc(mat(1:N/2,1:N/2))

keyboard

end
