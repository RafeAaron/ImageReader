#include "bmp.h"
#include<iostream>
#include<fstream>
#include <cstdint>

using namespace std;

bool flipRightLeft(unsigned char* data, int width)
{
    unsigned char newdata[width];

    for(int a = 0; a < width; a++)
    {

        newdata[a] = data[width - 1 - a];
    }

    for(int b = 0; b < width; b++)
    {
        data[b] = newdata[b];
    }

    return true;
}

bool flipUpDownImage(unsigned char* data, int width, int height)
{
    unsigned char newdata[width * height];

    for(int a = 0 ; a < height; a++)
    {

        for(int b = 0; b < width; b++)
        {
            newdata[(a * width) + b] = data[(((height - 1) - a) * width) + b];
        }
    }

    for(int d = 0; d < height; d++)
    {

        for(int c = 0; c < width; c++)
        {
            data[(d * width) + c] = newdata[(d * width) + c];
        }

    }

    return true;
}

BMP::BMP(char* fileName)
{
    ifstream file(fileName);
    
    if(!file.is_open())
    {
        cout << "Error reading file: " << fileName << endl;
        return;
    }
    else{

        for(int i = 0; i < 14; i++)
        {
            this->fileHeader[i] = file.get();
        }

        for(int i = 0; i < 40; i++)
        {
            this->DIBHeader[i] = file.get();
        }
        
        if(this->intializeVariablesFirst() && this->initializeVariablesSecond())
        {

            if(this->bitsPerPixel == 32 || this->bitsPerPixel == 24)
            {

                file.seekg(this->byteOffset, std::ios::beg);

                int total_bytes = this->width * this->height * (this->bitsPerPixel / 8);

                unsigned char byteData[total_bytes];

                for(int i = 0; i < total_bytes; i++)
                {
                    byteData[i] = file.get();
                }

                this->pixelData = byteData;

                this->sizeOfData = total_bytes;

                this->initialized = true;
                this->fileName = fileName;
                cout << "Initialization complete" << endl;
                
            }else{
                cout << "Unsupported bmp file. Retry with image that has either 24 or 32 bits per Pixel" << endl;
            }
        }

        else{

            cout << "There was an issue with the Initialization of the class" << endl;
        }
    }

    file.close();
}

bool BMP::intializeVariablesFirst()
{
    
    if(this->fileHeader[0] == 0x42 && this->fileHeader[1] == 0x4D)
    {
        isBMP = true;
    }
    else{
        isBMP = false;
        return false;
    }

    //Setting file Size Variable
    int file_S = 0;

    file_S = file_S << 8 | this->fileHeader[5];
    file_S = file_S << 8 | this->fileHeader[4];
    file_S = file_S << 8 | this->fileHeader[3];
    file_S = file_S << 8 | this->fileHeader[2];

    this->fileSize = file_S;

    //Setting the byte Offset
    int byte_O = 0;

    byte_O = byte_O << 8 | this->fileHeader[13];
    byte_O = byte_O << 8 | this->fileHeader[12];
    byte_O = byte_O << 8 | this->fileHeader[11];
    byte_O = byte_O << 8 | this->fileHeader[10];

    this->byteOffset = byte_O;

    return true;
}

bool BMP::initializeVariablesSecond()
{

    //Setting Width
    int width_i = 0;

    width_i = width_i << 8 | this->DIBHeader[7];
    width_i = width_i << 8 | this->DIBHeader[6];
    width_i = width_i << 8 | this->DIBHeader[5];
    width_i = width_i << 8 | this->DIBHeader[4];

    this->width = width_i;

    //Setting Height
    int height_i = 0;

    height_i = height_i << 8 | this->DIBHeader[11];
    height_i = height_i << 8 | this->DIBHeader[10];
    height_i = height_i << 8 | this->DIBHeader[9];
    height_i = height_i << 8 | this->DIBHeader[8];

    this->height = height_i;

    //Setting colorPanes
    int colorPanes_i = 0;

    colorPanes_i = colorPanes_i << 8 | this->DIBHeader[13];
    colorPanes_i = colorPanes_i << 8 | this->DIBHeader[12];

    this->colorPanes = colorPanes_i;

    //Setting number of bits per pixel
    int numberOfPixels_i = 0;

    numberOfPixels_i = numberOfPixels_i << 8 | this->DIBHeader[15];
    numberOfPixels_i = numberOfPixels_i << 8 | this->DIBHeader[14];

    this->bitsPerPixel = numberOfPixels_i;

    int compressionMethod_i = 0;

    compressionMethod_i = compressionMethod_i << 8 | this->DIBHeader[19];
    compressionMethod_i = compressionMethod_i << 8 | this->DIBHeader[18];
    compressionMethod_i = compressionMethod_i << 8 | this->DIBHeader[17];
    compressionMethod_i = compressionMethod_i << 8 | this->DIBHeader[16];

    this->compression = compressionMethod_i;

    return true;

}

unsigned char* BMP::retrieveMonoChrome32(int green, int blue, int red, int alpha, bool flipRight)
{

    int defaultgreen = 2;
    int defaultred = 2;
    int defaultblue = 2;
    int defaultalpha = 2;

    if(green > 0)
    {
        defaultgreen = green;
    }

    if(red > 0)
    {
        defaultred = red;
    }

    if(blue > 0)
    {
        defaultblue = blue;
    }

    if(alpha > 0)
    {
        defaultalpha = alpha;
    }

    int totalSize = this->width * (this->height - 7);
    unsigned char monoChromeData32[totalSize];

    for(int i = totalSize - 1; i >= 0; i--)
    {
        uint8_t red_i = this->pixelData[(4 * i) + 0];
        uint8_t green_i = this->pixelData[(4 * i) + 1];
        uint8_t blue_i = this->pixelData[(4 * i) + 2];
        uint8_t alpha_i = this->pixelData[(4 * i) + 3];

        if(alpha_i > defaultalpha || red_i > defaultred || blue > defaultblue || green > defaultgreen)
        {
            monoChromeData32[i] = 1;
        }
        else{
            monoChromeData32[i] = 0;
        }

    }

    unsigned char int_data[this->width];

    for(int c = 0; c < this->height - 7; c++)
    {

        for(int i = 0; i < this->width; i++)
        {
            int_data[i] = monoChromeData32[(c * width) + i];
        }

        if(flipRight)
        {
            if(flipRightLeft(int_data, this->width))
            {

                for(int i = 0; i < this->width; i++)
                {
                    monoChromeData32[(c * width) + i] = int_data[i];
                }
            }
        }
    }

    if(flipUpDownImage(monoChromeData32, this->width, this->height - 7))
    {
        cout << "Flipped Success" << endl;
    }

    for(int i = 0; i < totalSize; i++)
    {
        cout << (int)monoChromeData32[i];

        if((i + 1) % this->width == 0 && i != 0)
        {
            cout << endl;
        }

    }

    cout << endl;

    this->monoChrome_Int = monoChromeData32;

    return this->monoChrome_Int;
}

unsigned char* BMP::retrieveMonoChrome24(int green, int blue, int red, bool flipRight)
{


    int defaultgreen = 2;
    int defaultred = 2;
    int defaultblue = 2;

    if(green > 0)
    {
        defaultgreen = green;
    }

    if(red > 0)
    {
        defaultred = red;
    }

    if(blue > 0)
    {
        defaultblue = blue;
    }
    
    int totalSize = this->width * (this->height - 7);
    unsigned char monoChromeData24[totalSize];

    for(int i = totalSize - 1; i >= 0; i--)
    {
        uint8_t red_i = this->pixelData[(3 * i) + 0];
        uint8_t green_i = this->pixelData[(3 * i) + 1];
        uint8_t blue_i = this->pixelData[(3 * i) + 2];

        if(red_i > defaultred || blue_i > defaultblue || green_i > defaultgreen)
        {
            monoChromeData24[i] = 1;
        }
        else{
            monoChromeData24[i] = 0;
        }

    }

    unsigned char int_data[this->width];

    for(int c = 0; c < this->height - 7; c++)
    {

        for(int i = 0; i < this->width; i++)
        {
            int_data[i] = monoChromeData24[(c * width) + i];
        }

        if(flipRight)
        {
            if(flipRightLeft(int_data, this->width))
            {

                for(int i = 0; i < this->width; i++)
                {
                    monoChromeData24[(c * width) + i] = int_data[i];
                }
            }
        }
    }

    if(flipUpDownImage(monoChromeData24, this->width, this->height - 7))
    {
        cout << "Flipped Success" << endl;
    }

    for(int i = 0; i < totalSize; i++)
    {
        cout << (int)monoChromeData24[i];

        if((i + 1) % this->width == 0 && i != 0)
        {
            cout << endl;
        }

    }

    cout << endl;

    this->monoChrome_Int = monoChromeData24;

    return this->monoChrome_Int;
}

unsigned char* BMP::retrieveMonoChrome(int green, int blue, int red, int alpha, bool flipRight)
{

    if(this->bitsPerPixel == 32) return this->retrieveMonoChrome32(green, blue, red, alpha, flipRight);
    else return this->retrieveMonoChrome24(green, blue, red, flipRight);
}

unsigned char* BMP::returnTo1BitMonoChrome(int green, int blue, int red, int alpha, bool flipRight){

    int monoChromeWidth = -1;
    int divisibleBy8 = false;


    if(this->width % 8 == 0)
    {
        monoChromeWidth = this->width;
        divisibleBy8 = true;
    }else
    {
        monoChromeWidth = (((this->width / 8) + 1) * 8);
    }

    int finalSize = (monoChromeWidth / 8) * (height - 7) ;

    unsigned char monochromeBitmap[finalSize];

    unsigned char* monoChromeBitmapRaw = retrieveMonoChrome(green, blue, red, alpha, flipRight);
    int size = monoChromeWidth * (this->height - 7);
    int bytePosition = 0;

    if(divisibleBy8)
    {

        uint8_t byte = 0;

        for(int i = 0; i < size; i++)
        {

            if(monoChromeBitmapRaw[i] == 1)  byte = byte << 1 | 1;
            else byte = byte << 1 | 0;

            if((i + 1) % 8 == 0 && i != 0)
            {
                monochromeBitmap[bytePosition] = byte;

                cout << "Byte: " << (int)byte << " ";
                bytePosition++;
                byte = 0;
            }
        }
    }
    else{

        uint8_t byte;
        int width_i = 0;

        for(int i = 0; i < size; i++)
        {

            if(width_i < this->width)
            {
                if(monoChromeBitmapRaw[i] == 1)  byte = byte << 1 | 1;
                else byte = byte << 1 | 0;
                
                if((i + 1) % 8 == 0 && i != 0)
                {
                    monochromeBitmap[bytePosition] = byte;
                    bytePosition++;
                    byte = 0;
                    width_i++;
                }
            }
            else{

                int difference = monoChromeWidth - width_i;

                for(int b = 0; b < difference + 1; b++)
                {
                   byte = byte << 1 | 0; 
                }
                
                    monochromeBitmap[bytePosition] = byte;
                    bytePosition++;
                    byte = 0;
                    i--;
                    width_i = 0;
            }
        }
    }

    this->monoChromeBitmapFinalized = monochromeBitmap;
    return this->monoChromeBitmapFinalized;
}