#include "ico.h"
#include<iostream>
#include<fstream>
#include<cstdint>

using namespace std;

bool flipRightLeft2(unsigned char* data, int width)
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

bool flipUpDownImage2(unsigned char* data, int width, int height)
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

ICO::ICO(char* fileName)
{
    ifstream file(fileName);

    if(!file.is_open())
    {
        cout << "There was a problem locating the file" << endl;
        return;
    }

    this->fileName = fileName;

    file.seekg(0, std::ios::end);

    int end = file.tellg();

    file.seekg(0, std::ios::beg);

    cout << "End: " << end << endl;

    unsigned char fileData[end];

    for(int a = 0; a < end; a++)
    {
        fileData[a] = file.get();
    }

    this->fileData = fileData;

    file.seekg(0, std::ios::beg);

    unsigned char header[6];

    for(int i = 0; i < 6; i++)
    {
        header[i] = file.get();
    }

    this->header = header;

    //Getting Image Type;
    int type = 0;

    type = type << 8 | header[3];
    type = type << 8 | header[2];

    this->imageType = type;

    // Getting number of images
    int number = 0;

    number = number << 8 | header[5];
    number = number << 8 | header[4];

    this->numberOfImages = number;

    unsigned char iconDirectories[number * 16];

    for(int a = 0; a < number * 16; a++)
    {
        iconDirectories[a] = file.get();
    }

    this->icoDirectories = iconDirectories;

    int widths_i[this->numberOfImages];
    int heights_i[this->numberOfImages];
    int colorPaletes_i[this->numberOfImages];
    int colorPanes_i[this->numberOfImages];
    int bitsPerPixel_i[this->numberOfImages];
    int sizeOfData_i[this->numberOfImages];
    int offsets_i[this->numberOfImages];

    for(int a = 0; a < this->numberOfImages; a++)
    {
        //getting widths
        widths_i[a] = 0 | this->icoDirectories[16 * a];

        if(widths_i[a] == 0) widths_i[a] = 256;

        //Getting Heights
        heights_i[a] = 0 | this->icoDirectories[(16 * a) + 1];

        //colorPalletes
        colorPaletes_i[a] = 0 | this->icoDirectories[(16 * a) + 2];

        //colorPanes
        colorPanes_i[a] = 0 | this->icoDirectories[(16 * a) + 5];
        colorPanes_i[a] = colorPanes_i[a] << 8 | this->icoDirectories[(16 * a) + 4];

        // BitsPerPixel
        bitsPerPixel_i[a] = 0 | this->icoDirectories[(16 * a) + 7];
        bitsPerPixel_i[a] = bitsPerPixel_i[a] << 8 | this->icoDirectories[(16 * a) + 6];

        //sizes
        sizeOfData_i[a] = 0 | this->icoDirectories[(16 * a) + 11];
        sizeOfData_i[a] = sizeOfData_i[a] << 8 | this->icoDirectories[(16 * a) + 10];
        sizeOfData_i[a] = sizeOfData_i[a] << 8 | this->icoDirectories[(16 * a) + 9];
        sizeOfData_i[a] = sizeOfData_i[a] << 8 | this->icoDirectories[(16 * a) + 8];

        //offsets
        offsets_i[a] = 0 | this->icoDirectories[(16 * a) + 15];
        offsets_i[a] = offsets_i[a] << 8 | this->icoDirectories[(16 * a) + 14];
        offsets_i[a] = offsets_i[a] << 8 | this->icoDirectories[(16 * a) + 13];
        offsets_i[a] = offsets_i[a] << 8 | this->icoDirectories[(16 * a) + 12];

    }

    this->widths = widths_i;
    this->heights = heights_i;
    this->colorPaletes = colorPaletes_i;
    this->colorPanes = colorPanes_i;
    this->bitsPerPixel = bitsPerPixel_i;
    this->sizes = sizeOfData_i;
    this->offsets = offsets_i;

    cout << "Widths: " << this->widths[0] << endl;

    file.close();
}

int ICO::getWidth(int index)
{
    if(index > this->numberOfImages - 1)
    {
        cout << "Image index not present" << endl;
        return -1;
    }

    return this->widths[index];
}

int ICO::getHeight(int index)
{
    if(index > this->numberOfImages - 1)
    {
        cout << "Image index not present" << endl;
        return -1;
    }

    return this->heights[index];
}

int ICO::getOffset(int index)
{
    if(index > this->numberOfImages - 1)
    {
        cout << "Image index not present" << endl;
        return -1;
    }

    return this->offsets[index];
}

int ICO::getColorPalete(int index)
{
    if(index > this->numberOfImages - 1)
    {
        cout << "Image index not present" << endl;
        return -1;
    }

    return this->colorPaletes[index];
}

int ICO::getColorPane(int index)
{
    if(index > this->numberOfImages - 1)
    {
        cout << "Image index not present" << endl;
        return -1;
    }

    return this->colorPanes[index];
}

int ICO::getBitsPerPixel(int index)
{
    if(index > this->numberOfImages - 1)
    {
        cout << "Image index not present" << endl;
        return -1;
    }

    return this->bitsPerPixel[index];
}

int ICO::getSize(int index)
{
    if(index > this->numberOfImages - 1)
    {
        cout << "Image index not present" << endl;
        return -1;
    }

    return this->sizes[index];
}

unsigned char* ICO::getImageData(int index)
{

    if(index > this->numberOfImages - 1 ) cout << "Index out of range" << endl;

    int size = this->getSize(index);
    int offset = this->getOffset(index);

    unsigned char* imageData = new unsigned char[size];

    for(int a = 0; a < size; a++)
    {

        imageData[a] = this->fileData[offset + a];
    }

    cout << "Image data copied" << endl;

    return imageData;
}

unsigned char* ICO::getPixelData(int index, int width, int height, int bitsPerPixel)
{
    
    int offset = this->getOffset(index);

    int sizeOfHeader = 0;

    sizeOfHeader = sizeOfHeader | this->fileData[offset + 3];
    sizeOfHeader = sizeOfHeader << 8 | this->fileData[offset + 2]; 
    sizeOfHeader = sizeOfHeader << 8 | this->fileData[offset + 1]; 
    sizeOfHeader = sizeOfHeader << 8 | this->fileData[offset + 0]; 

    cout << "Size of data: " << sizeOfHeader << endl;

    int width_i = 0;

    width_i = width_i | this->fileData[offset + 7];
    width_i = width_i << 8 | this->fileData[offset + 6];
    width_i = width_i << 8 | this->fileData[offset + 5];
    width_i = width_i << 8 | this->fileData[offset + 4];

    cout << "Width: " << width_i << endl;

    int height_i = 0;

    height_i = height_i | this->fileData[offset + 7];
    height_i = height_i << 8 | this->fileData[offset + 6];
    height_i = height_i << 8 | this->fileData[offset + 5];
    height_i = height_i << 8 | this->fileData[offset + 4];

    cout << "Height: " << height_i << endl;

    int colorPanes_i = 0;

    colorPanes_i = colorPanes_i | this->fileData[offset + 8];
    colorPanes_i = colorPanes_i << 8 | this->fileData[offset + 9];

    cout << "Color Panes: " << colorPanes_i << endl;

    int sizeOfBytes = width * height * (bitsPerPixel / 8);

    unsigned char* data = new unsigned char[sizeOfBytes];

    int startingPoint = offset + 40;

    for(int d = 0; d < sizeOfBytes; d++)
    {
        data[d] = this->fileData[startingPoint + d];
    }

    this->sizeOfBytes = sizeOfBytes;

    return data;

}

unsigned char* ICO::getMonochromeDataIntermediate(int index, int green, int red, int blue, int alpha, bool flipRight, bool flipDown)
{

    int defaultgreen = 2;
    int defaultred = 2;
    int defaultblue = 2;
    int defaultalpha = 2;

    if(green > -1)
    {
        defaultgreen = green;
    }

    if(red > -1)
    {
        defaultred = red;
    }

    if(blue > -1)
    {
        defaultblue = blue;
    }

    if(alpha > -1)
    {
        defaultalpha = alpha;
    }

    int bitsPerPixel = this->getBitsPerPixel(index);

    if( bitsPerPixel != 32 && bitsPerPixel != 24)
    {
        unsigned char* error = new unsigned char[1];
        error[0] = -1;
        return error;
    }

    unsigned char* data_i = this->getPixelData(index, this->getWidth(index), this->getHeight(index), this->getBitsPerPixel(index));

    int totalSize = this->getWidth(index) * this->getHeight(index);
    unsigned char* monochromeData_i = new unsigned char[totalSize];

    if(this->getBitsPerPixel(index) == 32)
    {

        for(int c = 0; c < totalSize; c++)
        {
            uint8_t red_i = data_i[(4 * c) + 0];
            uint8_t green_i = data_i[(4 * c) + 1];
            uint8_t blue_i = data_i[(4 * c) + 2];
            uint8_t alpha_i = data_i[(4 * c) + 3];

            if(red_i > defaultred || blue_i > defaultblue || green_i > defaultgreen || alpha_i > defaultalpha)
            {
                monochromeData_i[c] = 1;
            }else
            {
                monochromeData_i[c] = 0;
            }
        }
    }
    else{

        for(int c = 0; c < totalSize; c++)
        {
            uint8_t red_i = data_i[(3 * c) + 0];
            uint8_t green_i = data_i[(3 * c) + 1];
            uint8_t blue_i = data_i[(3 * c) + 2];

            if(red_i > defaultred || blue_i > defaultblue || green_i > defaultgreen)
            {
                monochromeData_i[c] = 1;
            }else
            {
                monochromeData_i[c] = 0;
            }
        }
    }

    if(flipDown)
    {
        if(flipUpDownImage2(monochromeData_i, this->getWidth(index), this->getHeight(index)))
        {
            cout << "Image Flipped Right side up" << endl;
        }
        else{
            cout << "Image failed to flip" << endl;
        }
    }

    int width_i = this->getWidth(index);
    int height_i = this->getHeight(index);

    if(flipRight)
    {
        for(int a = 0; a < height_i; a++)
        {
            unsigned char int_data[width_i];

            for(int b = 0; b < width_i; b++)
            {
                int_data[b] = monochromeData_i[(a * width_i) + b];
            }

            flipRightLeft2(int_data, width_i);

            for(int b = 0; b < width_i; b++)
            {
                monochromeData_i[(a * width_i) + b] = int_data[b];
            }

        }
    }

    int width = this->getWidth(index);

    cout <<"Width: " <<  width << endl;

    for(int i = 0; i < totalSize; i++)
    {

        cout << (int)monochromeData_i[i];

        if((i + 1) % width == 0 && i != 0)
        {
            cout << endl;
        }
    }

    return monochromeData_i;

}

unsigned char* ICO::getMonoChromeRepresentation(int index, int green, int red, int blue, int alpha, bool flipRight, bool flipDown)
{

    int width_i = this->getWidth(index);
    int height_i = this->getHeight(index);

    unsigned char* dataFinal_i = this->getMonochromeDataIntermediate(index, green, red, blue, alpha, flipRight, flipDown);
    unsigned char* dataFinal;
    int position = 0;

    int width_f = -1;

    if(width_i % 8 == 0)
    {

        width_f = width_i;

        dataFinal = new unsigned char[(width_i / 8) * height_i];

        for(int a = 0; a < height_i; a++)
        {
            uint8_t byte = 0;

            for(int c = 0; c < width_i; c++)
            {

                if(dataFinal_i[(a * width_i) + c] == 1)
                {
                    byte = byte | 1;
                }else{
                    byte = byte | 0;
                }

                if((c + 1) % 8 == 0 && c != 0)
                {
                    dataFinal[position] = byte;
                    position++;
                    byte = 0;
                }

            }
        }
    }else{

        int width_i2 = ((width_i / 8) + 1 ) * 8;
        width_f = width_i2;

        dataFinal = new unsigned char[(width_i2 / 8) * height_i];

        for(int a = 0; a < height_i; a++)
        {
            uint8_t byte = 0;

            for(int c = 0; c < width_i2; c++)
            {

                if(c == width_i - 1)
                {
                    int difference = width_i2 - width_i;

                    for(int e = 0; e < difference; e++)
                    {
                        byte = byte | 0;
                    }

                    c = c - 1;
                }else{

                    if(dataFinal_i[(a * width_i) + c] == 1)
                    {
                        byte = byte | 1;
                    }else{
                        byte = byte | 0;
                    }
                }

                if((c + 1) % 8 == 0 && c != 0)
                {
                    dataFinal[position] = byte;
                    position++;
                    byte = 0;
                }

            }
        }
    }

    int totalSize_f = -1;

    if(width_f != width_i)
    {
        totalSize_f = width_f * height_i;
    }
    else{
        totalSize_f = width_i * height_i;
    }

    cout << "Final Width: " << width_f << endl;
    cout << "Final Height: " << height_i << endl;
    cout << "Final Number Of Bits:" << totalSize_f << endl;
    cout << "Final Number Of Bytes: " << totalSize_f / 8 << endl; 

    return dataFinal;

}