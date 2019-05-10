path = 'F:\大学学习资源\第六学期课程\硬件课程设计\screenTest1\image.txt';
A=uint16(load(path));

%将16bit颜色转换为24位的红绿蓝
[m, n]=size(A);
image = uint8(zeros(m, n, 3));

for i = 1:m
    for j = 1:n
        image(i,j,1) = bitand(bitshift( A(i,j), -11 ),31 )*8;%31 = 11111B
        image(i,j,2) = bitand(bitshift( A(i,j), -5 ),63 )*4;%63 = 111111B
        image(i,j,3) = bitand(A(i,j),31 )*8;%31 = 11111B
    end
end
figure(2);
margin = 10;%将图像外边填充厚度为margin的黑色，方便看到边界
imageBig = uint8(zeros(m+margin*2, n+margin*2, 3));
imageBig((margin+1):(margin+m),(margin+1):(margin+n),:) = image;
imshow(imageBig);