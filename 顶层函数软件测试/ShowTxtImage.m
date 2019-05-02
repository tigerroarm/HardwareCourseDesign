path = 'D:\hardwaredesign\ClinkMatlab\ClinkMatlab\image.txt';
A=load(path);
A=A*4;%change 666 form to 888 form%
[m, n]=size(A);
image = zeros(m, int32(n/3), 3);

for i = 1:int32(n/3)
    for j = 1:3
        image(:,i,j) = A(:, int32(3*(i-1)+j));
    end
end

imshow(image);