#include <stdio.h>
#include <stdlib.h>

int WriteArray2File(FILE* fp, unsigned short* pointer, int row, int column);
int WriteNumber(FILE* fp, int number);

int main()
{

    unsigned short array[4][4] = {0xfff0, 0xff0f, 0xff0f, 0xf0ff, 0x0fff};
    unsigned short* pointer = *array;
    FILE* fp = 0;

    fp = fopen("image.txt", "r+");

    if(fp == NULL)
        return(-1);
    else{
        WriteArray2File(fp, pointer, 4, 4);
        fclose(fp);
    }


    return 0;
}

int WriteArray2File(FILE* fp, unsigned short* pointer, int row, int column)
{
    int R, G, B = 0;
    int i = 0,j = 0;
    unsigned short test = 0;

    for(;i < row*column; i++){
    // print a row

        for(; j < column; j++){
        // print an element

            test = *(pointer + i);
            //get the value of RGB(in 565 form)

            R = (test>>11)&0x001f;
            G = (test>>5)&0x001f;
            B = test&0x001f;

            // transform RGB to 666 form
            R = ((R<<1)&0xfffe)|((R>>4)&0x0001);
            B = ((B<<1)&0xfffe)|((B>>4)&0x0001);

            //write a number to file
            WriteNumber(fp, R); putc(' ',fp);
            WriteNumber(fp, G); putc(' ',fp);
            WriteNumber(fp, B); putc(' ',fp);

            putc(' ',fp);
            i++;
        }

        putc('\n',fp);// newline
        j = 0;
    }

    putc(EOF,fp);// end of file

    return 0;
}

int WriteNumber(FILE* fp, int number)
{
    int char1, char2;

    char1 = number/10;
    char2 = number%10;

    putc(char1+ 48,fp);
    putc(char2+ 48,fp);

    return 0;
}
