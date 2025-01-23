#include<iostream>

using namespace std;

class BMP
{
    bool intializeVariablesFirst();
    bool initializeVariablesSecond();
    bool initializeVariablesThird();
    unsigned char* retrieveMonoChrome32(int green, int blue, int red, int alpha, bool flipRight);
    unsigned char* retrieveMonoChrome24(int green, int blue, int red, bool flipRight);

    public:
        char* fileName;
        unsigned char fileHeader[14];
        unsigned char DIBHeader[40];
        unsigned char* pixelData;
        unsigned char* monoChrome_Int;
        unsigned char* monoChromeBitmapFinalized;
        int sizeOfData = 0;
        bool isBMP = false;
        int fileSize = -1;
        int byteOffset = -1;
        bool initialized = false;
        int height = -1;
        int width = -1;
        int colorPanes = -1;
        int bitsPerPixel = -1;
        int compression = -1;

        BMP(char* fileName);
        unsigned char* retrieveMonoChrome(int green, int blue, int red, int alpha, bool flipRight);
        unsigned char* returnTo1BitMonoChrome(int green, int blue, int red, int alpha, bool flipRight);
};